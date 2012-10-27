from edge import Edge
from square_matrix import SquareMatrix

class UndirectedGraph:

    def __init__(self, size):
        self.adj_matrix = SquareMatrix(size)

    def vertexCount(self):
        return self.adj_matrix.size()

    def addEdge(self, vertex1, vertex2):
        self.adj_matrix.set(vertex1, vertex2, True)
        self.adj_matrix.set(vertex2, vertex1, True)

    def areConnected(self, vertex1, vertex2):
        return self.adj_matrix.get(vertex1, vertex2)

    def adjacentEdges(self, vertex):
        adj_edges = []
        for vertex2 in range(0, self.adj_matrix.size()):
            if self.areConnected(vertex, vertex2):
                adj_edges.append(Edge(vertex, vertex2))
        return adj_edges

    def edgeCandidates(self, tree, vertexDegrees):
        matrix_copy = self.adj_matrix.clone()
        for edge in tree:
            matrix_copy.set(edge.vertex1, edge.vertex2, False)
        #... todo

    def __str__(self):
        result = " |"
        for col in range(0, self.vertexCount()):
            result += " {0}".format(col)
        result += "\n"

        result += "-|"
        for i in range(0, self.vertexCount()):
            result += "--"
        result += "-\n"

        for row in range(0, self.vertexCount()):
            result += "{0}|".format(row)
            for col in range(0, self.vertexCount()):
                value = self.adj_matrix.get(row, col)
                result += " {0}".format(int(value))
            result += "\n"

        return result

