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

    def edgeCandidates(self, tree):
        """Returns a list of edges of this graph where one vertex of the edge
        is contained in the given tree and the other vertex is not."""
        matrix_copy = self.adj_matrix.clone()

        # remove edges already in tree from adjacency matrix
        for edge in tree.edges:
            matrix_copy.set(edge.vertex1, edge.vertex2, False)
            matrix_copy.set(edge.vertex2, edge.vertex1, False)

        for vertex_1 in range(0, self.adj_matrix.size()):
            # skip edges where the first vertex would have degree 0 in the tree
            if tree.vertexDegrees()[vertex_1] == 0:
                continue
            for vertex_2 in range(0, self.adj_matrix.size()):
                # skip edges that would create cycle in the tree
                if tree.vertexDegrees()[vertex_2] != 0:
                    continue
                if matrix_copy.get(vertex_1, vertex_2):
                    yield Edge(vertex_1, vertex_2)

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

