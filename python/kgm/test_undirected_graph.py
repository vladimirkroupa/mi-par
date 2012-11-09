import unittest
from edge import Edge
from spanning_tree import SpanningTree
from undirected_graph import UndirectedGraph

class TestUndirectedGraph(unittest.TestCase):

    def setUp(self):
        self.graph = UndirectedGraph(5)

    def test_vertexCount(self):
        vertices = self.graph.vertexCount()

        self.assertTrue(vertices == 5)

    def test_addEdge(self):
        self.graph.addEdge(1, 4)
        self.graph.addEdge(0, 4)
        self.assertTrue(self.graph.areConnected(0, 4))
        self.assertTrue(self.graph.areConnected(4, 0))
        self.assertTrue(self.graph.areConnected(1, 4))
        self.assertTrue(self.graph.areConnected(4, 1))
        self.assertFalse(self.graph.areConnected(0, 1))

    def test_adjacentEdges(self):
        self.graph.addEdge(0, 1)
        self.graph.addEdge(0, 3)
        self.graph.addEdge(1, 4)

        adj_0 = self.graph.adjacentEdges(0)
        adj_1 = self.graph.adjacentEdges(1)

        self.assertEqual([Edge(0, 1), Edge(0, 3)], adj_0)
        self.assertEqual([Edge(0, 1), Edge(1, 4)], adj_1)

    def test_edgeCandidates(self):
        self.graph.addEdge(0, 1)
        self.graph.addEdge(1, 2)
        self.graph.addEdge(1, 3)
        self.graph.addEdge(1, 4)
        self.graph.addEdge(2, 3)
        self.graph.addEdge(0, 4)

        tree = SpanningTree(5)
        tree.addEdge(Edge(0, 1))

        actual = self.graph.edgeCandidates(tree)
        expected = [Edge(1, 2), Edge(1, 3), Edge(1, 4), Edge(0, 4)]
        self.assertEqual(set(expected), set(actual))

    def test_str(self):
        self.graph.addEdge(0, 1)
        self.graph.addEdge(0, 3)
        self.graph.addEdge(1, 4)

        str = self.graph.__str__()
        expected = """ | 0 1 2 3 4\n-|-----------\n0| 0 1 0 1 0\n1| 1 0 0 0 1\n2| 0 0 0 0 0\n3| 1 0 0 0 0\n4| 0 1 0 0 0\n"""

        self.assertEqual(expected, str)

    if __name__ == '__main__':
        unittest.main()