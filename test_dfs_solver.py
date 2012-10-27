import unittest
from dfs_solver import DFSSolver
from undirected_graph import UndirectedGraph
from edge import Edge

class TestDFSSolver(unittest.TestCase):

    def setUp(self):
        graph = UndirectedGraph(7)
        graph.addEdge(0, 1)
        graph.addEdge(1, 2)
        graph.addEdge(1, 3)
        graph.addEdge(1, 4)
        graph.addEdge(4, 5)
        graph.addEdge(4, 6)
        graph.addEdge(0, 4)

        self.solver = DFSSolver(graph)

    def test_firstEdgeCandidates(self):
        edges = self.solver.firstEdgeCandidates()

        self.assertEqual(set([Edge(0, 1), Edge(0, 4)]), set(edges))

    def test_backtrackMarker(self):
        self.assertTrue(self.solver.isBacktrackMarker(Edge(-1, -1)))

    def test_findBestSolution(self):
        solution, price = self.solver.findBestSolution()
        self.assertEqual(3, price)
        expected = [Edge(0, 1), Edge(1, 2), Edge(1, 3), Edge(0, 4), Edge(4, 5), Edge(4, 6)]
        self.assertEqual(set(expected), set(solution.edgeList()))

if __name__ == '__main__':
        unittest.main()