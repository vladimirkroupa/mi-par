import copy
import numpy

class SquareMatrix:

    def __init__(self, size):
        self.matrix = numpy.zeros(shape=(size, size))

    def get(self, row, col):
        return self.matrix[row, col]

    def set(self, row, col, value):
        self.matrix[row, col] = value

    def size(self):
        return self.matrix.shape[0]

    def clone(self):
        return copy.deepcopy(self)