from operator import xor
from edge import Edge

class SpanningTree:

    def __init__(self, graphSize):

        self.edges = []
        self.vertex_degrees = []
        for i in range(0, graphSize):
            self.vertex_degrees.append(0)

    def edgeList(self):
        return self.edges

    def vertexDegrees(self):
        return self.vertex_degrees

    def addEdge(self, edge):
        self.vertex_degrees[edge.vertex1] += 1
        self.vertex_degrees[edge.vertex2] += 1
        self.edges.append(edge)

    def removeLastEdge(self):
        removed = self.edges.pop()
        self.vertex_degrees[removed.vertex1] -= 1
        self.vertex_degrees[removed.vertex2] -= 1

    def edgeCount(self):
        return len(self.edges)

    def maxDegree(self):
        return max(self.vertex_degrees)

    def maxDegreeWith(self, edge):
        max = self.vertex_degrees[0]

        for i in range(0, len(self.vertex_degrees)):
            degree = self.vertex_degrees[i]
            if (edge.vertex1 == i) or (edge.vertex2 == i):
                degree += 1
            if degree > max:
                max = degree

        return max

    def __str__(self):
        result = ", ".join(str(edge) for edge in self.edges)
        result += "\ndegree: {0}".format(self.maxDegree())
        return result