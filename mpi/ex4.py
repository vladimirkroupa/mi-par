from mpi4py import MPI

HELLO_TAG = 1

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()

if my_rank != 0:
    message = "Greetings from process {0}!".format(my_rank)
    comm.send(message, dest=0, tag=HELLO_TAG)
else:
    size = comm.Get_size()
    print("There are {0} processes. ".format(size))
    for i in range(1, size):
        status = MPI.Status()
        has_message = comm.Iprobe(source=MPI.ANY_SOURCE,tag=MPI.ANY_TAG, status=status)
        if has_message:
            print("Node {0} has a message.".format(my_rank))
            data = comm.recv(source=MPI.ANY_SOURCE, tag=HELLO_TAG, status=status)
            source = status.Get_source()
            print("Node {0} retrieved a message from {1}: '{2}'".format(my_rank, source, data))
        else:
            print("No messages for node {0}.".format(my_rank))

MPI.Finalize()
