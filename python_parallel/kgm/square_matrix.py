import copy
import numpy

class SquareMatrix:
    """Square matrix for storing numeric or boolean values. Backed by numpy array."""

    def __init__(self, size):
        """Creates a new square matrix.
        :param size: Size of the matrix to be created.
        """
        self.matrix = numpy.zeros(shape=(size, size))

    def get(self, row, col):
        """Retrieves an element from the matrix.

        :param row: row position.
        :param col: column position.
        :return: en element at specified position.
        """
        return self.matrix[row, col]

    def set(self, row, col, value):
        """Stores an element in the matrix.

        :param row: row position.
        :param col: col position.
        :param value: value to store. Must be numeric or boolean.
        """
        self.matrix[row, col] = value

    def size(self):
        """
        :return: size of the matrix.
        """
        return self.matrix.shape[0]

    def clone(self):
        """
        :return: clone of the matrix.
        """
        return copy.deepcopy(self)