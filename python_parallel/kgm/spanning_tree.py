class SpanningTree:
    """Spanning tree of a graph. Optimized for memory efficiency, so it does not store the whole graph it belongs to.
     The intended usage is to exchange instance of this class between solvers working on a same undirected graph.
    """

    def __init__(self, graphSize):
        """Creates an empty spanning tree for a graph of given size.
        :param graphSize: size of the graph the spanning tree belongs to.
        """
        self.edges = []
        self.vertex_degrees = [0] * graphSize

    def edgeList(self):
        """
        :return: list of edges in the spanning tree.
        """
        return self.edges

    def vertexDegrees(self):
        return self.vertex_degrees

    def addEdge(self, edge):
        """Adds a new graph edge to the spanning tree. Does not check if the edge exists in the graph!
        If the edge is already present in the spanning tree, the tree won't be changed.
        Has complexity of O(n).
        :param edge: edge to add to the spanning tree.
        """
        if edge not in self.edges:
            self.vertex_degrees[edge.vertex1] += 1
            self.vertex_degrees[edge.vertex2] += 1
            self.edges.append(edge)

    def removeLastEdge(self):
        """Removes the last added edge. Can be called repeatedly until the spanning tree is empty.
        Call on an empty spanning tree has no effect.
        """
        if self.edges:
            removed = self.edges.pop()
            self.vertex_degrees[removed.vertex1] -= 1
            self.vertex_degrees[removed.vertex2] -= 1

    def edgeCount(self):
        """
        :return: number of edges in the spanning tree.
        """
        return len(self.edges)

    def maxDegree(self):
        """
        :return: maximum vertex degree of the spanning tree.
        """
        return max(self.vertex_degrees)

    def maxDegreeWith(self, edge):
        """
        Returns maximum vertex degree of a spanning tree consisting of this spanning tree with given edge added.
        Does not modify the spanning tree.
        :param edge: edge to add..
        :return: maximum degree of the hypothetical tree.
        """
        max = self.vertex_degrees[0]

        for i in range(len(self.vertex_degrees)):
            degree = self.vertex_degrees[i]
            if (edge.vertex1 == i) or (edge.vertex2 == i):
                degree += 1
            if degree > max:
                max = degree

        return max

    def __str__(self):
        result = ", ".join(str(edge) for edge in self.edges)
        return result