/* 
 * File:   main.cpp
 * Author: janstadler
 *
 * Created on 14. říjen 2012, 16:08
 */

#include <cstdlib>
#include <stdio.h>
#include "UndirectedGraph.h"
#include "DFSSolver.h"

using namespace std;

UndirectedGraph * readGraphFromFile(char * filename) {
    char token;
    int x=0, y=0, i=0;
    int nodes_count = 0;
    UndirectedGraph *graph;
    
    FILE *file = fopen(filename, (const char *)"r");
    if (file == NULL) {
        std::cout << "Neexistujici soubor:" << filename << endl;
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nodes_count) != 1) {
        std:cout << "Nelze nacist prvni radek vstupniho souboru" <<endl;
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

/*
 * 
 */
int main(int argc, char** argv) {
    int i;

    if (argc != 2) {
        cout << "Spatny pocet parametru" << endl;
        cout << "binarka nazev-souboru" << endl;
        cout << std::endl << "Arguments:" << endl;
        exit(EXIT_FAILURE);
    }

    UndirectedGraph *graph = readGraphFromFile(argv[1]);
    cout << "vertex count=" << graph->vertexCount() << endl;

    DFSSolver *solver = new DFSSolver(graph);
    pair<vector<Edge> *, int>  *result = solver->findBestSolution();
	vector<Edge> *solution = result->first;
	int solutionPrice = result->second;
    if (solution != NULL) {
        cout << "Best solution:" << endl;
        for (i = 0; i < solution->size(); i++) {
            cout << solution->at(i) << endl;
        }
		cout << "Spanning tree degree: " << solutionPrice << endl;
    }

	delete graph;
    delete solution;
	delete result;
	delete solver; // tady to umre

    return 0;
}

