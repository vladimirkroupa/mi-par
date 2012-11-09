from mpi4py import MPI
import numpy

HELLO_TAG = 1
TIMES = 5
SIZE = 1000

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()
nodes = comm.Get_size()
params = numpy.empty(2, dtype='i')

if my_rank != 0:
    print("Process {0} waiting to receive params...".format(my_rank))
    comm.Recv(params, source=MPI.ANY_SOURCE, tag=HELLO_TAG)
    print("Process {0} received: times={1}, size={2}.".format(my_rank, params[0], params[1]))
else:
    params = numpy.array([TIMES, SIZE], dtype='i')
    for dest in range(1, nodes):
        print("Process 0 is sending params to process {0}.".format(dest))
        comm.Send(params, dest=dest, tag=HELLO_TAG)

comm.Barrier()
print("{0} after barrier.".format(my_rank))

message_out = numpy.array([1] * params[1], dtype='i')
destination = (my_rank + 1) % nodes
source = my_rank - 1
if source < 0:
    source = nodes - 1
print("Process {0} will send data to process {1} and will receive data from process {2}.".format(my_rank, source, destination))

message_in = numpy.empty(params[1], dtype='i')

for i in range(0, params[0]):
    request = comm.Isend(message_in, dest=destination, tag=HELLO_TAG)
    print("Process {0} sent data to process {1}.".format(my_rank, destination))
    received = False
    while not received:
        received = comm.Iprobe(source=MPI.ANY_SOURCE,tag=HELLO_TAG)
        #print "Process {0} is probing for incoming messages...".format(my_rank, destination)
    status = MPI.Status()
    comm.Recv(message_in, source=source, tag=HELLO_TAG, status=status)
    print("Process {0} received data from process {1}.".format(my_rank, status.Get_source()))

    sent = False
    while not sent:
        sent = request.Test()
        print("Process {0} is waiting for ISend to complete...".format(my_rank))
    print("{0}: ISend completed.".format(my_rank))


MPI.Finalize()