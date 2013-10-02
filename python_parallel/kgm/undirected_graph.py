from edge import Edge
from square_matrix import SquareMatrix

class UndirectedGraph:
    """Undirected unweighted graph for representing graph structure only. Does not store any values in vertices.
    Vertices are numbered starting with zero.
    """

    def __init__(self, size):
        """
        Creates a new graph with specified number of vertices. The number of vertices is immutable..
        :param size: number of vertices in the graph.
        """
        self.adj_matrix = SquareMatrix(size)

    def vertexCount(self):
        """
        :return: number of vertices in the graph.
        """
        return self.adj_matrix.size()

    def addEdge(self, vertex1, vertex2):
        """Adds an edge to the graph. The order of vertices doesn't matter.

        :param vertex1: first node of the edge (from 0 to size - 1).
        :param vertex2: second node of the edge (from 0 to size - 1).
        """
        self.adj_matrix.set(vertex1, vertex2, True)
        self.adj_matrix.set(vertex2, vertex1, True)

    def areConnected(self, vertex1, vertex2):
        """Checks if there is an edge between two vertices. The order of vertices doesn't matter.
        :param vertex1: first vertex (from 0 to size - 1).
        :param vertex2: second vertex (from 0 to size - 1).
        :return: True if the edge exists in the graph, false otherwise.
        """
        return self.adj_matrix.get(vertex1, vertex2)

    def adjacentEdges(self, vertex):
        """Returns a set of edges adjacent to the given vertex.

        :param vertex: endpoint vertex.
        :return: set of edges adjacent to the vertex.
        """
        adj_edges = set()
        for vertex2 in range(0, self.adj_matrix.size()):
            if self.areConnected(vertex, vertex2):
                adj_edges.add(Edge(vertex, vertex2))
        return adj_edges

    def edgeCandidates(self, tree):
        """Returns a set of possible edges of this graph where each edge has one of its vertices
        contained in the given tree and the other vertex not.

        :param tree: Tree UndirectedGraph instance.
        :return:
        """
        matrix_copy = self.adj_matrix.clone()

        # remove edges already in tree from adjacency matrix
        for edge in tree.edges:
            matrix_copy.set(edge.vertex1, edge.vertex2, False)
            matrix_copy.set(edge.vertex2, edge.vertex1, False)

        candidates = set()

        for vertex_1 in range(self.adj_matrix.size()):
            # skip edges where the first vertex would have degree 0 in the tree
            if tree.vertexDegrees()[vertex_1] == 0:
                continue
            for vertex_2 in range(0, self.adj_matrix.size()):
                # skip edges that would create cycle in the tree
                if tree.vertexDegrees()[vertex_2] != 0:
                    continue
                if matrix_copy.get(vertex_1, vertex_2):
                    candidates.add(Edge(vertex_1, vertex_2))

        return candidates

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

