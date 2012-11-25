import copy
import sys
import logging
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
        self.logger = self.setupLogging(debug)
        self.mpi_logger = self.logger

        self.total_edges = 0
        self.total_solutions = 0

    def setupLogging(self, debug):
        logger = logging.getLogger(str(self.rank))
        handler = logging.FileHandler('./log/{}.txt'.format(self.rank))
        formatter = logging.Formatter(fmt='%(asctime)s: %(message)s', datefmt='%H:%M:%S')
        handler.setFormatter(formatter)
        logger.addHandler(handler)
        if debug:
            logger.setLevel(logging.DEBUG)
        return logger

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

            self.logger.debug(self.printStack(self.edge_stack))

            self.expand()

        self.logger.info("%s processed %s edges.", self.rank, self.total_edges)
        self.logger.info("%s found %s solutions.", self.rank, self.total_solutions)

        # DFS traversal completed
        if self.foundSolution():
            return self.best, self.best_price
        else:
            self.logger.info("%s: no solution found.", self.rank)
            return None, sys.maxint

    def expand(self):
        current_edge = self.edge_stack.pop()
        self.total_edges += 1

        if current_edge.isBacktrackMarker():
            # found backtrack marker, remove last edge from spanning tree
            self.spanning_tree.removeLastEdge()
        else:
            # add current edge to spanning tree
            self.spanning_tree.addEdge(current_edge)

            self.logger.debug(self.spanning_tree)

            if self.isSolution():

                self.logger.debug("%s found solution!", self.rank)
                self.total_solutions += 1

                price = self.spanning_tree.maxDegree()
                if self.isBestPossible(price):
                    # current solution is the best possible, return
                    self.updateBest(price)
                    self.logger.debug("! %s found the best solution possible", self.rank)
                    self.askToTerminate()
                elif self.isBestSoFar(price):
                    # better that any solution so far, update best
                    self.updateBest(price)
                    self.logger.debug("%s found new solution with price %s:",self.rank, price)
                    self.logger.debug(self.spanning_tree)
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
                        self.logger.debug("Leaving out edge %s", edge)

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
        self.mpi_logger.debug("0 has %s initial work parts", len(parts))
        toNode = 1
        for part in parts:
            self.comm.send(part, dest=toNode, tag=DFSSolver.WORK_SHARE)
            self.mpi_logger.debug("0 has sent initial work to %s", toNode)
            toNode += 1

    def acceptInitialWork(self):
        status = MPI.Status()
        # check for shared work
        work_resp = self.comm.Iprobe(source=0, tag=DFSSolver.WORK_SHARE, status=status)
        if work_resp:
            self.mpi_logger.debug("%s has initial work", self.rank)
            self.receiveWork(0)
        else:
            self.mpi_logger.debug("%s has no initial work :(", self.rank)

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
            self.mpi_logger.debug("! %s exiting.", self.rank)
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
            self.mpi_logger.debug("%s has WORK_REQ from %s", self.rank, source)
            self.comm.recv(source=source, tag=DFSSolver.WORK_REQ, status=status)
            self.sendWork(source)

    def sendWork(self, to):
        new_stack, new_spanning_tree = self.splitWork()
        if new_stack is None and new_spanning_tree is None:
            return

        self.comm.send((new_stack, new_spanning_tree), dest=to, tag=DFSSolver.WORK_SHARE)
        self.checkColorChange(to)
        self.mpi_logger.debug("%s sent work offer to %s", self.rank, to)

    def rejectAll(self):
        status = MPI.Status()
        has_work_req = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REQ, status=status)
        while has_work_req:
            source = status.Get_source()
            self.comm.recv(source=source, tag=DFSSolver.WORK_REQ)
            self.comm.send(dest=source, tag=DFSSolver.WORK_REJECT)
            self.mpi_logger.debug("%s received and REJECTED work request from %s", self.rank, source)
            has_work_req = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REQ, status=status)

    def sendWorkRequest(self):
        destination = self.nextNode()
        self.comm.send(dest=destination, tag=DFSSolver.WORK_REQ)
        self.mpi_logger.debug("%s sent work request to %s", self.rank, destination)

    def prevNode(self):
        if self.rank == 0:
            return self.comm_size - 1
        else:
            return self.rank - 1

    def nextNode(self):
        return (self.rank + 1) % self.comm_size

    def checkWorkResponse(self):
        status = MPI.Status()
        # check for shared work
        work_resp = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_SHARE, status=status)
        if work_resp:
            source = status.Get_source()
            self.mpi_logger.debug("there is shared work for %s from %s", self.rank, source)
            return True, source, True
        else:
            # check for rejections
            rejection = self.comm.Iprobe(source=MPI.ANY_SOURCE, tag=DFSSolver.WORK_REJECT, status=status)
            if rejection:
                # got rejection for work request
                source = status.Get_source()
                self.mpi_logger.debug("%s received work request rejection from %s", self.rank, source)
                self.comm.recv(source=source, tag=DFSSolver.WORK_REJECT)
                return False, None, False
            else:
                # no reponse
                return False, None, True

    def receiveWork(self, source):
        work_tuple = self.comm.recv(source=source, tag=DFSSolver.WORK_SHARE)
        self.mpi_logger.debug("%s RECEIVED work from %s", self.rank, source)
        new_stack = work_tuple[0]
        new_spanning_tree = work_tuple[1]

        self.mpi_logger.debug("%s received stack: %s", self.rank, self.printStack(new_stack))
        self.mpi_logger.debug("%s received tree: %s", self.rank, new_spanning_tree)

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
            self.mpi_logger.debug("%s sent TERMINATION tag to %s", self.rank, node)
            self.comm.send(dest=node, tag=DFSSolver.TERMINATE)

    def handleTokens(self):

        def initialTokenSend(self):
            token = Token()
            sendToken(self, token)
            self.token_sent = True;

        def sendToken(self, token):
            self.comm.send(token, dest=self.nextNode(), tag=DFSSolver.TOKEN)
            self.color = Token.WHITE
            self.mpi_logger.debug("%s sent %s to %s", self.rank, token, self.nextNode())

        def receiveToken(self):
            token = self.comm.recv(source=self.prevNode(), tag=DFSSolver.TOKEN)
            self.mpi_logger.debug("%s received %s from %s", self.rank, token, self.prevNode())
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
            #print("No token for {}").format(self.rank)
            if not self.initial_token_sent and self.rank == 0:
               initialTokenSend(self)

    def checkTerminationMsg(self):
        should_terminate = self.comm.Iprobe(source=0, tag=DFSSolver.TERMINATE)
        if should_terminate:
            self.comm.recv(source=0, tag=DFSSolver.TERMINATE)
            self.mpi_logger.debug("%s has received termination token.", self.rank)
            self.finished = True
            return

class Token:

    WHITE = 0
    BLACK = 1

    def __init__(self):
        self.color = Token.WHITE

    def __str__(self):
        return "white token" if self.color == 0 else "black token"
