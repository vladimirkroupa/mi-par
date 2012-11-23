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
    TOKEN = 4
    TERMINATE = 5

    def __init__(self, graph, comm, debug=False):
        self.graph = graph
        self.spanning_tree = SpanningTree(graph.vertexCount())
        self.edge_stack = []
        self.best_price = sys.maxint
        self.best = None

        self.color = Token.BLACK
        self.initial_token_sent = False
        self.counter = 0
        self.finished = False

        self.rank = comm.Get_rank()
        self.comm_size = comm.Get_size()
        self.comm = comm

        self.debug = debug
        self.mpi_debug = debug

    def findBestSolution(self):
        # push all edges adjacent to vertex 0 to stack
        if self.rank == 0:
            for edge in self.firstEdgeCandidates():
                self.edge_stack.append(edge)
            self.distributeInitialWork()

        self.comm.Barrier()

        if self.rank != 0:
            self.acceptInitialWork()

        # main loop
        while not self.shouldTerminate():

            # trochu prasarna
            if len(self.edge_stack) == 0:
                continue

            if self.debug:
                print(self.printStack(self.edge_stack))

            self.expand()

        # DFS traversal completed
        if self.foundSolution():
            return self.best, self.best_price
        else:
            if self.debug:
                print("{0}: no solution found.").format(self.rank)
            return None, sys.maxint

    def expand(self):
        current_edge = self.edge_stack.pop()

        if current_edge.isBacktrackMarker():
            # found backtrack marker, remove last edge from spanning tree
            self.spanning_tree.removeLastEdge()
        else:
            # add current edge to spanning tree
            self.spanning_tree.addEdge(current_edge)

            if self.debug:
                print(self.spanning_tree)

            if self.isSolution():

                if self.debug:
                    print("{0} found solution!").format(self.rank)

                price = self.spanning_tree.maxDegree()
                if self.isBestPossible(price):
                    # current solution is the best possible, return
                    self.updateBest(price)
                    if self.debug:
                        print("* {0} found the best solution possible").format(self.rank)
                    self.askToTerminate()
                elif self.isBestSoFar(price):
                    # better that any solution so far, update best
                    self.updateBest(price)
                    if self.debug:
                        print("{0} found new solution with price {1}:").format(self.rank, price)
                        print(self.spanning_tree)
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

    def firstEdgeCandidates(self):
        vertex = 0
        return self.graph.adjacentEdges(vertex)

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
            if edge.isBacktrackMarker():
                result += "|   **   |\n"
            else:
                result += "| {0:6} |\n".format(edge)
        result += "^        ^"
        return result

    def distributeInitialWork(self):
        parts = self.initialWorkSplit(self.comm_size - 1)
        print("0 has {0} initial work parts ***".format(len(parts)))
        toNode = 1
        for part in parts:
            self.comm.send(part, dest=toNode, tag=DFSSolver.WORK_SHARE)
            if self.mpi_debug:
                print("0 has sent initial work to {0}".format(toNode))
            toNode += 1

    def acceptInitialWork(self):
        status = MPI.Status()
        # check for shared work
        work_resp = self.comm.Iprobe(source=0, tag=DFSSolver.WORK_SHARE, status=status)
        if work_resp:
            if self.mpi_debug:
                print("{0} has initial work".format(self.rank))
            self.receiveWork(0)
        else:
            if self.mpi_debug:
                print("{0} has no initial work :(".format(self.rank))

    def initialWorkSplit(self, partsReq):
        parts = []
        partCnt = 0
        while partCnt < partsReq:
            edgesLeft = self.countEdgesOnStack()
            while edgesLeft < partsReq + 1 - partCnt:
                if edgesLeft == 0:
                    return parts
                self.expand()
                edgesLeft = self.countEdgesOnStack()
            new_stack, new_tree = self.splitWork()
            parts.append((new_stack, new_tree))
            partCnt += 1
        return parts

    def countEdgesOnStack(self):
        count = 0
        for edge in self.edge_stack:
            if not edge.isBacktrackMarker():
                count += 1
        return count

    def splitWork(self):

        def fromBottomElement(self):
            for index, edge in enumerate(self.edge_stack):
                if not edge.isBacktrackMarker():
                    return index
            return None

        def countBacktracks(self, to_index):
            count = 0
            for edge in self.edge_stack[to_index + 1:]:
                if edge.isBacktrackMarker():
                    count += 1
            return count

        elem_to_move = fromBottomElement(self)
        if elem_to_move is None:
            return None, None
        new_stack = self.edge_stack[:elem_to_move + 1]
        del self.edge_stack[elem_to_move]

        backtracks_to_do = countBacktracks(self, elem_to_move)
        new_spanning_tree = copy.deepcopy(self.spanning_tree)
        for i in range(backtracks_to_do):
            new_spanning_tree.removeLastEdge()

        return new_stack, new_spanning_tree

    def shouldTerminate(self):
        if self.counter > 100:
            self.counter %= 100
            return False
        else:
            self.counter += 1

        self.checkTerminationMsg()
        if self.finished:
            if self.mpi_debug:
                print("* {0} exiting...").format(self.rank)
            return True

        if self.hasWorkToShare():
            self.handleWorkRequests()
            return False
        else:
            # print("{0} has no work to share.").format(self.rank)
            # not enough work to share
            self.rejectAll()
        if len(self.edge_stack) > 0:
            # but enough work to continue
            return False
        else:
            # own stack is empty

            if self.comm_size == 1:
                return True

            self.handleTokens()

            work_request_sent = False
            while True:
                self.rejectAll()

                self.handleTokens()
                if self.finished:
                    break

                if not work_request_sent:
                    self.sendWorkRequest()
                    work_request_sent = True
                else:
                    work_available, avl_from, work_req_sent_flag = self.checkWorkResponse()
                    if work_req_sent_flag is not None:
                        work_request_sent = work_req_sent_flag
                    if work_available:
                        self.receiveWork(avl_from)
                        return False

    def hasWorkToShare(self):
        edge_count = 0
        for edge in self.edge_stack:
            if not edge.isBacktrackMarker():
                edge_count += 1
        return edge_count >= 2

    def handleWorkRequests(self):
        status = MPI.Status()
        # check for work request from anyone
        has_message = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REQ, status=status)
        if not has_message:
            # print("{0} has no work requests.").format(self.rank)
            return
        else:
            source = status.Get_source()
            if self.mpi_debug:
                print("{0} has WORK_REQ from {1}").format(self.rank, source)
            self.comm.recv(source=source, tag=DFSSolver.WORK_REQ, status=status)
            self.sendWork(source)

    def sendWork(self, to):
        new_stack, new_spanning_tree = self.splitWork()
        if new_stack is None and new_spanning_tree is None:
            return

        self.comm.send((new_stack, new_spanning_tree), dest=to, tag=DFSSolver.WORK_SHARE)
        self.checkColorChange(to)
        if self.mpi_debug:
            print("{0} sent work offer to {1}").format(self.rank, to)

    def rejectAll(self):
        status = MPI.Status()
        has_work_req = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REQ, status=status)
        while has_work_req:
            source = status.Get_source()
            self.comm.recv(source=source, tag=DFSSolver.WORK_REQ)
            self.comm.send(dest=source, tag=DFSSolver.WORK_REJECT)
            if self.mpi_debug:
                print("{0} received and REJECTED work request from {1}").format(self.rank, source)
            has_work_req = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REQ, status=status)

    def sendWorkRequest(self):
        destination = self.nextNode()
        self.comm.send(dest=destination, tag=DFSSolver.WORK_REQ)
        if self.mpi_debug:
            print("{0} sent work request to {1}").format(self.rank, destination)

    def prevNode(self):
        if self.rank == 0:
            return self.comm_size - 1
        else:
            return self.rank - 1

    def nextNode(self):
        return (self.rank + 1) % self.comm_size

    def checkWorkResponse(self):
        status = MPI.Status()
        # muzu tady mit ANY_SOURCE?
        # check for shared work
        work_resp = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_SHARE, status=status)
        if work_resp:
            source = status.Get_source()
            if self.mpi_debug:
                print("there is shared work for {0} from {1}").format(self.rank, source)
            return True, source, True
        else:
            # check for rejections
            rejection = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REJECT, status=status)
            if rejection:
                # got rejection for work request
                source = status.Get_source()
                if self.mpi_debug:
                    print("{0} received work request rejection from {1}").format(self.rank, source)
                self.comm.recv(source=source, tag=DFSSolver.WORK_REJECT)
                return False, None, False
            else:
                # no reponse
                return False, None, True

    def receiveWork(self, source):
        work_tuple = self.comm.recv(source=source, tag=DFSSolver.WORK_SHARE)
        if self.mpi_debug:
            print("{0} RECEIVED work from {1}").format(self.rank, source)
        new_stack = work_tuple[0]
        new_spanning_tree = work_tuple[1]

        if self.mpi_debug:
            print("{0} received stack: {1}").format(self.rank, self.printStack(new_stack))
            print("{0} received tree: {1}").format(self.rank, new_spanning_tree)

        self.spanning_tree = new_spanning_tree
        for edge in new_stack:
            self.edge_stack.append(edge)

    def checkColorChange(self, sentWorkTo):
        change_color = False
        if self.rank == 0 and sentWorkTo == self.comm_size - 1:
            # should not happen
            change_color = True
        elif self.rank > sentWorkTo:
            change_color = True

        if change_color:
            self.color = Token.BLACK

    def askToTerminate(self):
        for node in range(0, self.comm_size):
            if self.mpi_debug:
                print("* {0} sent TERMINATION tag to {1}").format(self.rank, node)
            self.comm.send(dest=node, tag=DFSSolver.TERMINATE)

    def handleTokens(self):

        def initialTokenSend(self):
            token = Token()
            sendToken(self, token)
            self.token_sent = True;

        def sendToken(self, token):
            self.comm.send(token, dest=self.nextNode(), tag=DFSSolver.TOKEN)
            self.color = Token.WHITE
            if self.mpi_debug:
                print("* {0} sent {1} to {2}").format(self.rank, token, self.nextNode())

        def receiveToken(self):
            token = self.comm.recv(source=self.prevNode(), tag=DFSSolver.TOKEN)
            if self.mpi_debug:
                print("* {0} received {1} from {2}").format(self.rank, token, self.prevNode())
            if self.rank == 0:
                if token.color == Token.WHITE:
                    self.askToTerminate()
                else:
                    # black token arrived, try again
                    initialTokenSend(self)
            else:
                token.color = self.color
                sendToken(self, token)

        self.checkTerminationMsg()

        has_token = self.comm.Iprobe(source=self.prevNode(), tag=DFSSolver.TOKEN)
        if has_token:
            receiveToken(self)
        else:
            #print("* No token for {0}").format(self.rank)
            if not self.initial_token_sent and self.rank == 0:
               initialTokenSend(self)

    def checkTerminationMsg(self):
        should_terminate = self.comm.Iprobe(source=0, tag=DFSSolver.TERMINATE)
        if should_terminate:
            self.comm.recv(source=0, tag=DFSSolver.TERMINATE)
            if self.mpi_debug:
                print("* {0} has received termination token.").format(self.rank)
            self.finished = True
            return

class Token:

    WHITE = 0
    BLACK = 1

    def __init__(self):
        self.color = Token.WHITE

    def __str__(self):
        return "white token" if self.color == 0 else "black token"
