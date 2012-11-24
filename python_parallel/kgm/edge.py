class Edge:

    def __init__(self, vertex1, vertex2):
        self.vertex1 = vertex1
        self.vertex2 = vertex2

    def isBacktrackMarker(self,):
        return self.vertex1 == -1 and self.vertex2 == -1

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return "({0}, {1})".format(self.vertex1, self.vertex2)

    def __eq__(self, other):
        sameOrder = self.vertex1 == other.vertex1 and self.vertex2 == other.vertex2
        crossedOrder = self.vertex1 == other.vertex2 and self.vertex2 == other.vertex1
        return sameOrder or crossedOrder

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(self.vertex1) * hash(self.vertex2)
