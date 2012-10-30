import copy

class SquareMatrix:

    def __init__(self, size):
        self.rows = [[0 for col in range(size)] for row in range(size)]

    def get(self, row, col):
        return self.rows[row][col]

    def set(self, row, col, value):
        self.rows[row][col] = value

    def size(self):
        return len(self.rows)

    def clone(self):
        return copy.deepcopy(self)