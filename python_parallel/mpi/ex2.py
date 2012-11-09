from mpi4py import MPI

HELLO_TAG = 11
MASTER = 0

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()

if my_rank != MASTER:
    status = MPI.Status()
    data = comm.recv(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=status)
    comm.send(data, dest=MASTER, tag=HELLO_TAG)
    print("Node {0} received a message from node {1} and sent it back.".format(my_rank, status.Get_source()))
else:
    size = comm.Get_size()
    print("There are {0} processes. ".format(size))
    message = "Vy jste ty hustci, co diktujou KOS? Mam pro vas mesaz z budoucnosti."
    for dest in range(1, size):
        comm.send(message, dest)
        print("Node {0} sent a new message to node {1}.".format(my_rank, dest))

    for i in range(1, size):
        status = MPI.Status()
        reply = comm.recv(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=status)
        print("Node {0} received message: '{1}' from node {2}.".format(my_rank, reply, status.Get_source()))

MPI.Finalize()
