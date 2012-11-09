from mpi4py import MPI

HELLO_TAG = 11
MASTER = 0

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()

received = comm.bcast("Rumrburt", root=MASTER)
print("Node {0} received broadcast message: '{1}'.".format(my_rank, received))

if my_rank != MASTER:
    comm.send(received, dest=MASTER, tag=HELLO_TAG)
    print("Node {0} sent a message to node {1}.".format(my_rank, MASTER))
else:
    size = comm.Get_size()

    for i in range(1, size):
        status = MPI.Status()
        reply = comm.recv(source=MPI.ANY_SOURCE, tag=HELLO_TAG, status=status)
        print("Node {0} received message: '{1}' from node {2}.".format(my_rank, reply, status.Get_source()))

MPI.Finalize()
