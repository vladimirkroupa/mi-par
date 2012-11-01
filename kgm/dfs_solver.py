import copy
from edge import Edge
from spanning_tree import SpanningTree

class DFSSolver:

    BEST_PRICE_POSSIBLE = 2

    def __init__(self, graph, debug=False):
        self.graph = graph
        self.spanning_tree = SpanningTree(graph.vertexCount())
        self.edge_stack = []
        self.best_price = 0
        self.best = None
        self.debug = debug

    def findBestSolution(self):
        # push all edges adjacent to vertex 0 to stack
        for edge in self.firstEdgeCandidates():
            self.edge_stack.append(edge)

        # main loop
        while len(self.edge_stack) > 0:

            if self.debug:
                self.printStack()

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

    def printStack(self):
        print ("|--------|")
        for edge in self.edge_stack:
            if self.isBacktrackMarker(edge):
                print("|   **   |")
            else:
                print("| {0:6} |".format(edge))
        print ("^        ^\n")

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
        for i in range(0, backtracks_to_do):
            new_spanning_tree.removeLastEdge()

        return new_stack, new_spanning_tree
