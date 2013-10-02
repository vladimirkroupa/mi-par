class Edge:
    """A single edge in an undirected graph.
    """

    def __init__(self, vertex1, vertex2):
        """Creates a new edge. Order of the vertices doesn't matter.
        :param vertex1: number of the first vertex (0 - (length of graph - 1))
        :param vertex2: number of the second vertex1 (0 - (length of graph - 1))
        """
        if vertex1 > vertex2:
            vertex1, vertex2 = vertex2, vertex1
        self.vertex1 = vertex1
        self.vertex2 = vertex2

    def isBacktrackMarker(self):
        """Minimum spanning tree algorithm specific method.
        :return: True if the edge is an instance of the backtrack market.
        """
        return self.vertex1 == -1 and self.vertex2 == -1

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return "({0}, {1})".format(self.vertex1, self.vertex2)

    def __eq__(self, other):
        sameOrder = self.vertex1 == other.vertex1 and self.vertex2 == other.vertex2
        crossedOrder = self.vertex1 == other.vertex2 and self.vertex2 == other.vertex1
        return sameOrder or crossedOrder

    def __lt__(self, other):
        return (10 * self.vertex1 + self.vertex2) < (10 * other.vertex1 + other.vertex2)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(self.vertex1) * hash(self.vertex2)
