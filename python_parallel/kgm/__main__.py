import argparse
from dfs_solver import DFSSolver
from undirected_graph import UndirectedGraph
from mpi4py import MPI
import sys

def main():
    comm = MPI.COMM_WORLD
    my_rank = comm.Get_rank()
    if (my_rank == 0):
        parser = argparse.ArgumentParser()
        parser.add_argument("file", help="input file with graph adjacency matrix")
        parser.add_argument("-d", "--debug", help="print additional messages", action="store_true")
        args = parser.parse_args()
        file = args.file
        graph = readGraph(file)
    else:
        graph = None
    received = comm.bcast(graph)
    solver = DFSSolver(received, comm, False)

    comm.Barrier()

    solution, price = solver.findBestSolution()
    sys.stdout.flush()

    comm.Barrier()

    # print("? {1}'s price is {0}").format(price, my_rank)

    min_price = comm.reduce(sendobj=price, op=MPI.MIN, root=0)
    if my_rank == 0:
        print("*** Minimum price is {0}.").format(min_price)
    if price == min_price:
        print("-------------------------------------")
        print("{0}: Minimum spanning tree, degree = {1}:").format(my_rank, price)
        print(solution)
        print("*************************************")

    MPI.Finalize()

def readGraph(file):
    graph_file =  open(file, 'r')
    graph = None

    for line_no, line in enumerate(graph_file):
        if line_no == 0:
            matrix_size = int(line)
            graph = UndirectedGraph(matrix_size)
        else:
            line = line.strip()
            for col_no, col in enumerate(line):
                if col == "1":
                    graph.addEdge(line_no - 1, col_no)
        line_no += 1

    graph_file.close()
    return graph

if  __name__ =='__main__':
    main()