import unittest
from edge import Edge

class TestEdge(unittest.TestCase):

    def setUp(self):
        self.edge = Edge(1, 2)
        self.edge2 = Edge(2, 1)

    def test_str(self):
        edge_str = self.edge.__str__()
        self.assertEqual("(1, 2)", edge_str)

        edge_str_2 = self.edge2.__str__()
        self.assertEqual("(2, 1)", edge_str_2)

    def test_eq_ne(self):
        self.assertTrue(self.edge == self.edge2)
        self.assertFalse(self.edge != self.edge2)

        edge3 = Edge(1, 2)
        self.assertTrue(self.edge == edge3)
        self.assertFalse(self.edge != edge3)

        edge4 = Edge(1, 3)
        self.assertFalse(edge3 == edge4)
        self.assertTrue(edge3 != edge4)


if __name__ == '__main__':
    unittest.main()