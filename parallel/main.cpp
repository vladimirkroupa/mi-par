/* 
 * File:   main.cpp
 * Author: janstadler
 *
 * Created on 14. říjen 2012, 16:08
 */

#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include "UndirectedGraph.h"
#include "DFSSolver.h"
#include "Packer.h"
#include "Logger.h"

using namespace std;

UndirectedGraph * readGraphFromFile(char * filename) {
    char token;
    int x=0, y=0, i=0;
    int nodes_count = 0;
    UndirectedGraph *graph;
    
    FILE *file = fopen(filename, (const char *)"r");
    if (file == NULL) {
        cout << "Neexistujici soubor:" << filename << endl;
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nodes_count) != 1) {
        cout << "Nelze nacist prvni radek vstupniho souboru" << endl;
        exit(EXIT_FAILURE);
    }
    
    graph = new UndirectedGraph(nodes_count);
    while (fscanf(file, "%c", &token) == 1) {
        if (token == '\r') {
            continue;
        }
        if (token == '\n') {
            //printf("\n");
            if (i > 0) {
                x=0;
                y++;
            }
            
            continue;
        }
        if(token == '1') {
            graph->addEdge(x,y);
        }
        x++;
        i++;
    }
	fclose(file);
    return graph;
}

//int main(int argc, char** argv) {
//	MPI_Init(&argc, &argv);
//
//	DFSSolver * solver = new DFSSolver(new UndirectedGraph(1));
//
//	vector<Edge> * stack = new vector<Edge>();
//	stack->push_back(Edge(1, 2));
//	stack->push_back(Edge(3, 4));
//	solver->printStack(stack);
//
//	SpanningTree * tree = new SpanningTree(5);
//	tree->addEdge(Edge(1, 2));
//	tree->addEdge(Edge(1, 2));
//	solver->printSpanningTree(tree);
//
//	pair<vector<Edge>*, SpanningTree *> * input = new pair<vector<Edge>*, SpanningTree *>(stack, tree);
//
//	char * humr = Packer::packWorkShare(input);
//
//	input = Packer::unpackWorkShare(humr);
//	solver->printSpanningTree(input->second);
//	solver->printStack(input->first);
//
//	MPI_Finalize();
//}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Spatny pocet parametru" << endl;
        cout << "binarka nazev-souboru" << endl;
        cout << std::endl << "Arguments:" << endl;
        exit(EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);
    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    Logger::setRank(myRank);

    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    cout << commSize << " nodes" << endl;

    UndirectedGraph * graph;
    int vertexCount;
    if (myRank == 0) {
        graph = readGraphFromFile(argv[1]);
        cout << "vertex count=" << graph->vertexCount() << endl;
        vertexCount = graph->vertexCount();
    }
    MPI_Bcast(&vertexCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int elemCnt = vertexCount * vertexCount;
    bool * matrixElems = new bool[elemCnt];
    if (myRank == 0) {
    	for (int i = 0; i < elemCnt; i++) {
    		matrixElems[i] = graph->getMatrixElem(i);
    	}
    }
    MPI_Bcast(matrixElems, elemCnt, MPI_INT, 0, MPI_COMM_WORLD);
    if (myRank != 0) {
    	SquareMatrix * matrix = new SquareMatrix(vertexCount, matrixElems);
    	graph = new UndirectedGraph(matrix);
    }
    delete[] matrixElems;

    DFSSolver * solver = new DFSSolver(graph);

    MPI_Barrier(MPI_COMM_WORLD);

    pair<vector<Edge> *, int>  * result = solver->findBestSolution();
	vector<Edge> * solution = result->first;
	int solutionPrice = result->second;

	MPI_Barrier(MPI_COMM_WORLD);

    int localMin[2];
    localMin[0] = solutionPrice;
    localMin[1] = myRank;
    int globalMin[2];
	MPI_Allreduce(&localMin, &globalMin, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);

	int winner = globalMin[1];
	int minPrice = globalMin[0];

	{ stringstream str; str << "Winner is: " << winner << endl << "Min price is: " << minPrice << endl; Logger::log(&str); }
	if (myRank == winner) {
		stringstream str;
		if (solution != NULL) {
			str << "Best solution:" << endl;
			for (unsigned i = 0; i < solution->size(); i++) {
				str << (*solution)[i] << endl;
			}
			str << "Spanning tree degree: " << solutionPrice << " (" << minPrice << ")" << endl;
		} else {
			str << "No solution found." << endl;
		}
		Logger::log(&str);
	}

	MPI_Finalize();

	Logger::logLn("MPI finalized");
	delete graph;
	Logger::logLn("graph deleted");
    if (solution != NULL) {
    	delete solution;
    	Logger::logLn("solution deleted");
    }
	delete result;
	Logger::logLn("result deleted");
	delete solver;
	Logger::logLn("solver deleted");

    return 0;
}



