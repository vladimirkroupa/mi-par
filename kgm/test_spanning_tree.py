import unittest
from spanning_tree import SpanningTree
from edge import Edge

class TestSpanningTree(unittest.TestCase):

    def setUp(self):
        self.spanning_tree = SpanningTree(7)

    def test_manipulateEdges(self):
        self.spanning_tree.addEdge(Edge(0, 1))
        self.assertEqual(1, self.spanning_tree.maxDegree())
        self.assertEqual(1, self.spanning_tree.edgeCount())

        self.spanning_tree.addEdge(Edge(1, 2))
        self.assertEqual(2, self.spanning_tree.maxDegree())
        self.assertEqual(2, self.spanning_tree.edgeCount())

        self.spanning_tree.removeLastEdge()
        self.assertEqual(1, self.spanning_tree.maxDegree())
        self.assertEqual(1, self.spanning_tree.edgeCount())

        self.spanning_tree.removeLastEdge()
        self.assertEqual(0, self.spanning_tree.maxDegree())
        self.assertEqual(0, self.spanning_tree.edgeCount())

    def test_maxDegree(self):
        self.spanning_tree.addEdge(Edge(0, 1))
        self.spanning_tree.addEdge(Edge(1, 2))
        self.spanning_tree.addEdge(Edge(1, 3))

        self.assertEqual(3, self.spanning_tree.maxDegree())

    def test_maxDegreeWith(self):
        self.spanning_tree.addEdge(Edge(0, 1))
        self.spanning_tree.addEdge(Edge(1, 2))

        degree = self.spanning_tree.maxDegreeWith(Edge(1, 3))
        self.assertEqual(3, degree)

    def test_str(self):
        self.spanning_tree.addEdge(Edge(0, 1))
        self.spanning_tree.addEdge(Edge(1, 2))
        self.spanning_tree.addEdge(Edge(1, 3))

        expected = "(0, 1), (1, 2), (1, 3)\ndegree: 3"
        actual = self.spanning_tree.__str__()
        self.assertEqual(expected, actual)

if __name__ == '__main__':
        unittest.main()