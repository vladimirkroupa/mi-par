from mpi4py import MPI

HELLO_TAG = 1

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()

if my_rank != 0:
    message = "Greetings from process {0}!".format(my_rank)
    comm.send(message, dest=0, tag=HELLO_TAG)
else:
    size = comm.Get_size()
    print("There are {0} processes. ").format(size)
    for i in range(1, size):
        status = MPI.Status()

        data = comm.recv(source=MPI.ANY_SOURCE, tag=HELLO_TAG, status=status)
        source = status.Get_source()
        print("Message from {0}: '{1}'").format(source, data)

MPI.Finalize()