/* 
 * File:   main.cpp
 * Author: janstadler
 *
 * Created on 14. říjen 2012, 16:08
 */

#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <string>
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

bool mpiDebugOn(char** argv, int i) {
	return (! strcmp(argv[i], "mpi_debug"));
}

void setMpiDebugOn() {
	DFSSolver::MPI_DEBUG = true;
	cout << "mpi_debug on" << endl;
}

bool seqDebugOn(char** argv, int i) {
	return (! strcmp(argv[i], "seq_debug"));
}

void setSeqDebugOn() {
	DFSSolver::DEBUG = true;
	cout << "seq_debug on" << endl;
}

bool logCoutOn(char** argv, int i) {
	return (! strcmp(argv[i], "log_cout"));
}

void setLogCoutOn() {
	Logger::TO_FILE = false;
	cout << "debug on cout" << endl;
}


int main(int argc, char** argv) {
    if (argc < 3 || argc > 6) {
        cout << "Usage: kgm graph_file sequential_steps [mpi_debug] [seq_debug] [log_cout]" << endl;
        exit(EXIT_FAILURE);
    }
    int seqSteps = atoi(argv[2]);
    if (! seqSteps) {
    	cout << "sequential_steps must be > 0." << endl;
    	exit(EXIT_FAILURE);
    }
    cout << "seq. steps: " << seqSteps << endl;
    bool mpiDebug = false;
    bool seqDebug = false;
    bool debugCout = false;
    if (argc > 3) {
    	mpiDebug |= mpiDebugOn(argv, 3);
    	seqDebug |= seqDebugOn(argv, 3);
    	debugCout |= logCoutOn(argv, 3);
    }
    if (argc > 4) {
    	mpiDebug |= mpiDebugOn(argv, 4);
    	seqDebug |= seqDebugOn(argv, 4);
    	debugCout |= logCoutOn(argv, 4);
    }
    if (argc > 5) {
    	mpiDebug |= mpiDebugOn(argv, 5);
    	seqDebug |= seqDebugOn(argv, 5);
    	debugCout |= logCoutOn(argv, 5);
    }
    if (mpiDebug) {
    	setMpiDebugOn();
    }
    if (seqDebug) {
    	setSeqDebugOn();
    }
    if (debugCout) {
    	setLogCoutOn();
    }
    cout << mpiDebug << ":" << seqDebug << ":" << debugCout << endl;

    MPI_Init(&argc, &argv);
    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    Logger::setRank(myRank);

    int commSize;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    cout << commSize << " nodes" << endl;

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    UndirectedGraph * graph;
    int vertexCount;
    if (myRank == 0) {
        graph = readGraphFromFile(argv[1]);
        cout << "vertex count=" << graph->vertexCount() << endl;
        vertexCount = graph->vertexCount();
    }
    MPI_Bcast(&vertexCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int elemCnt = vertexCount * vertexCount;
    int * matrixElems = new int[elemCnt];
    if (myRank == 0) {
    	for (int i = 0; i < elemCnt; i++) {
    		matrixElems[i] = graph->getMatrixElem(i);
    	}
    }
    // Logger::logLn("Broadcasting matrix elements...");
    MPI_Bcast(matrixElems, elemCnt, MPI_INT, 0, MPI_COMM_WORLD);
    // Logger::logLn("Broadcasted.");
    if (myRank != 0) {
    	SquareMatrix * matrix = new SquareMatrix(vertexCount, matrixElems);
    	graph = new UndirectedGraph(matrix);
    	// Logger::logLn("Created graph from matrix elements.");
    }
    delete[] matrixElems;

    DFSSolver * solver = new DFSSolver(graph, seqSteps);

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
			cout << "Best solution:" << endl;
			for (unsigned i = 0; i < solution->size(); i++) {
				str << (*solution)[i] << endl;
				cout << (*solution)[i] << endl;
			}
			str << "Spanning tree degree: " << solutionPrice << " (" << minPrice << ")" << endl;
			cout << "Spanning tree degree: " << solutionPrice << " (" << minPrice << ")" << endl;
		} else {
			str << "No solution found." << endl;
			cout << "No solution found." << endl;
		}
		Logger::log(&str);
	}

    MPI_Barrier(MPI_COMM_WORLD);

    double t2 = MPI_Wtime();
    if (myRank == 0) {
    	cout << "Time spent: " << t2 - t1 << "s" << endl;
    }

	MPI_Finalize();

	Logger::logLn("MPI finalized");
	delete graph;
	// Logger::logLn("graph deleted");
    if (solution != NULL) {
    	delete solution;
    	// Logger::logLn("solution deleted");
    }
	delete result;
	// Logger::logLn("result deleted");
	delete solver;
	// Logger::logLn("solver deleted");

    return 0;
}



