from mpi4py import MPI

class Rumr:

    def __init__(self):
        self.a_list = [1, 2]
        self.number = 4.1
        self.message = "mesaz z budoucnosti"

    def __str__(self):
        return "{0}, {1}, {2}".format(self.a_list, self.number, self.message)

HELLO_TAG = 1

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()

if my_rank != 0:
    data = Rumr()
    comm.send(data, dest=0, tag=HELLO_TAG)
else:
    size = comm.Get_size()
    print("There are {0} processes. ".format(size))
    for i in range(1, size):
        status = MPI.Status()

        data = comm.recv(source=MPI.ANY_SOURCE, tag=HELLO_TAG, status=status)
        source = status.Get_source()
        print("Message from {0}: '{1}'".format(source, data))

MPI.Finalize()