import argparse
from dfs_solver import DFSSolver
from undirected_graph import UndirectedGraph

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="input file with graph adjacency matrix")
    parser.add_argument("-d", "--debug", help="print additional messages", action="store_true")
    args = parser.parse_args()
    file = args.file
    graph = readGraph(file)
    solver = DFSSolver(graph, args.debug)
    solution, price = solver.findBestSolution()
    if solution == None:
        print("No spanning tree found.")
    else:
        print("Minimum spanning tree, degree = {0}:".format(price))
        print(solution)

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