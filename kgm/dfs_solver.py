import copy
import sys
from edge import Edge
from spanning_tree import SpanningTree
from mpi4py import MPI

class DFSSolver:

    BEST_PRICE_POSSIBLE = 2

    WORK_REQ = 1
    WORK_SHARE = 2
    WORK_REJECT = 3

    def __init__(self, graph, comm, debug=False):
        self.graph = graph
        self.spanning_tree = SpanningTree(graph.vertexCount())
        self.edge_stack = []
        self.best_price = sys.maxint
        self.best = None
        self.debug = False # = debug
        self.mpi_debug = debug

        self.comm = comm
        self.rank = self.comm.Get_rank()
        self.comm_size = self.comm.Get_size()

    def findBestSolution(self):
        # push all edges adjacent to vertex 0 to stack
        if self.rank == 0:
            for edge in self.firstEdgeCandidates():
                self.edge_stack.append(edge)

        # main loop
        while not self.shouldTerminate():

            if self.debug:
                print(self.printStack(self.edge_stack))

            current_edge = self.edge_stack.pop()
            if self.isBacktrackMarker(current_edge):
                # found backtrack marker, remove last edge from spanning tree
                self.spanning_tree.removeLastEdge()
            else:
                # add current edge to spanning tree
                self.spanning_tree.addEdge(current_edge)

                if self.debug:
                    print(self.spanning_tree)

                if self.isSolution():

                    if self.debug:
                        print("Found solution!")

                    price = self.spanning_tree.maxDegree()
                    if self.isBestPossible(price):
                        # current solution is the best possible, return
                        self.updateBest(price)
                        return self.best, self.best_price
                    elif self.isBestSoFar(price):
                        # better that any solution so far, update best
                        self.updateBest(price)
                    # since we've found the solution, current edge is a leaf of the DFS tree -> backtrack
                    self.spanning_tree.removeLastEdge()
                else:
                    # add backtrack marker to stack so we will know when we are moving up the DFS tree
                    self.pushBacktrackMarker()
                    # find new edges to add to spanning tree
                    for edge in self.graph.edgeCandidates(self.spanning_tree):
                        if self.possibleWinner(edge):
                            # candidate edge can lead to better solution than best solution so far
                            self.edge_stack.append(edge)
                        else:
                            if self.debug:
                                print("Leaving out edge {}".format(edge))

        # DFS traversal completed
        if self.foundSolution():
            return self.best, self.best_price
        else:
            if self.debug:
                print("No solution found.")
            return None, None

    def firstEdgeCandidates(self):
        vertex = 0
        return self.graph.adjacentEdges(vertex)

    def isBacktrackMarker(self, edge):
        return edge == Edge(-1, -1)

    def pushBacktrackMarker(self):
        self.edge_stack.append(Edge(-1, -1))

    def possibleWinner(self, edge):
        price = self.spanning_tree.maxDegreeWith(edge)
        return self.isBestSoFar(price)

    def isSolution(self):
        tree_edges = self.spanning_tree.edgeCount()
        graph_vertices = self.graph.vertexCount()
        return tree_edges == graph_vertices - 1

    def isBestPossible(self, price):
        return price == self.BEST_PRICE_POSSIBLE

    def isBestSoFar(self, price):
        if self.best == None:
            return True
        return price < self.best_price

    def updateBest(self, price):
        self.best_price = price
        self.best = copy.deepcopy(self.spanning_tree)

    def foundSolution(self):
        return self.best != None

    def printStack(self, stack):
        result = "\n|--------|\n"
        for edge in stack:
            if self.isBacktrackMarker(edge):
                result += "|   **   |\n"
            else:
                result += "| {0:6} |\n".format(edge)
        result += "^        ^"
        return result

    def splitWork(self):

        def fromBottomElement(self):
            for index, edge in enumerate(self.edge_stack):
                if not self.isBacktrackMarker(edge):
                    return index
            return None

        def countBacktracks(self, to_index):
            count = 0
            for edge in self.edge_stack[to_index + 1:]:
                if self.isBacktrackMarker(edge):
                    count += 1
            return count

        elem_to_move = fromBottomElement(self)
        new_stack = self.edge_stack[:elem_to_move + 1]
        del self.edge_stack[elem_to_move]

        backtracks_to_do = countBacktracks(self, elem_to_move)
        new_spanning_tree = copy.deepcopy(self.spanning_tree)
        for i in range(backtracks_to_do):
            new_spanning_tree.removeLastEdge()

        return new_stack, new_spanning_tree

    def shouldTerminate(self):
        if self.workToSplit():
            self.handleWorkRequests()
            return False
        else:
            print("{0} has no work to share.").format(self.rank)
            # not enough work to share
            self.rejectAll()
        if len(self.edge_stack) > 0:
            # but enough work to continue
            return False
        else:
            # own stack is empty
            if self.comm_size == 1:
                return True
            work_request_sent = False

            cnt = 0

            while True:
                self.rejectAll()
                # todo: sdileni nejelepsiho?
                # todo: ukoncovani
                cnt += 1
                if cnt > 500:
                    return True

                if not work_request_sent:
                    self.sendWorkRequest()
                    work_request_sent = True
                else:
                    work_available, avl_from, work_req_sent_flag = self.checkWorkResponse()
                    if work_req_sent_flag:
                        work_request_sent = work_req_sent_flag
                    if work_available:
                        self.receiveWork(avl_from)
                        return False


    def workToSplit(self):
        return len(self.edge_stack) > 3

    def handleWorkRequests(self):
        status = MPI.Status()
        # check for work request from anyone
        has_message = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=self.WORK_REQ, status=status)
        if not has_message:
            print("{0} has no work requests.").format(self.rank)
            return
        else:
            source = status.Get_source()
            print("{0} has WORK_REQ from {1}").format(self.rank, source)
            self.comm.recv(source=source, tag=self.WORK_REQ, status=status)
            self.sendWork(source)

    def sendWork(self, to):
        new_stack, new_spanning_tree = self.splitWork()
        self.comm.send((new_stack, new_spanning_tree), dest=to, tag=self.WORK_SHARE)
        print("{0} sent work offer to {1}").format(self.rank, to)

    def rejectAll(self):
        status = MPI.Status()
        has_message = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=self.WORK_REQ, status=status)
        while has_message:
            source = status.Get_source()
            self.comm.recv(source=source, tag=self.WORK_REQ)
            self.comm.send(dest=source, tag=self.WORK_REJECT)
            print("{0} received and REJECTED work request from {1}").format(self.rank, source)

            has_message = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=self.WORK_REQ, status=status)

    def sendWorkRequest(self):
        destination = (self.rank + 1) % self.comm_size
        self.comm.send(dest=destination, tag=self.WORK_REQ)
        print("{0} sent work request to {1}").format(self.rank, destination)

    def checkWorkResponse(self):
        status = MPI.Status()
        # muzu tady mit ANY_SOURCE?
        # check for shared work
        work_resp = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=self.WORK_SHARE, status=status)
        if work_resp:
            source = status.Get_source()
            print("there is shared work for {0} from {1}").format(self.rank, source)
            return True, source, None
        else:
            # check for rejections
            rejection = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=self.WORK_REJECT, status=status)
            if rejection:
                # got rejection for work request
                source = status.Get_source()
                print("{0} received work request rejection from {1}").format(self.rank, source)
                self.comm.recv(source=source, tag=self.WORK_REJECT)
                return False, None, False
            else:
                # no reponse
                return False, None, True

    def receiveWork(self, source):
        work_tuple = self.comm.recv(source=source, tag=self.WORK_SHARE)
        print("{0} RECEIVED work from {1}").format(self.rank, source)
        new_stack = work_tuple[0]
        new_spanning_tree = work_tuple[1]

        print("{0} received stack: {1}").format(self.rank, self.printStack(new_stack))
        print("{0} received tree: {1}").format(self.rank, new_spanning_tree)

        self.spanning_tree = new_spanning_tree
        for edge in new_stack:
            self.edge_stack.append(edge)

