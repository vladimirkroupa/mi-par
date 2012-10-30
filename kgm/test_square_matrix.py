import unittest
from square_matrix import SquareMatrix

class TestSquareMatrix(unittest.TestCase):

    def setUp(self):
        self.matrix = SquareMatrix(5)

    def test_empty(self):
        for i in range(5):
            for j in range(5):
                element = self.matrix.get(i, j)
                self.assertTrue(element == 0)

    def test_set_get(self):
        self.matrix.set(3, 3, 1)
        elem = self.matrix.get(3, 3)
        self.assertTrue(elem == 1)


    def test_sizes(self):
        size = self.matrix.size()
        self.assertTrue(size == 5)

    def test_clone(self):
        self.matrix.set(2, 2, 1)
        clone = self.matrix.clone()
        self.matrix.set(3, 3, 1)
        clone.set(4, 4, 1)

        orig_before_cloning = self.matrix.get(2, 2)
        self.assertTrue(orig_before_cloning == 1)

        orig_after_cloning = self.matrix.get(3, 3)
        matching_clone_elem = clone.get(3, 3)
        self.assertTrue(orig_after_cloning == 1)
        self.assertTrue(matching_clone_elem == 0)

        modified_clone = clone.get(4, 4)
        matching_orig_elem = self.matrix.get(4, 4)
        self.assertTrue(modified_clone == 1)
        self.assertTrue(matching_orig_elem == 0)


if __name__ == '__main__':
    unittest.main()