from mpi4py import MPI
from random import randrange

class Rumr:

    def __init__(self):
        self.a_list = [1, 2]
        self.number = 4.1
        self.message = "mesaz z budoucnosti"

    def __str__(self):
        return "{0}, {1}, {2}".format(self.a_list, self.number, self.message)

HELLO_TAG = 1

comm = MPI.COMM_WORLD

value = randrange(100)
print("My value is {0}.")

max = comm.reduce(sendobj=value, op=MPI.MAX, root=0)

my_rank = comm.Get_rank()
if my_rank == 0:
    print("Maximum is {0}.".format(max))

MPI.Finalize()