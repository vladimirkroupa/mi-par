/* 
 * File:   UndirectedGraph.h
 * 
 */

#ifndef UNDIRECTEDGRAPH_H
#define	UNDIRECTEDGRAPH_H

#include "Edge.h"
#include "SquareMatrix.h"
#include "SpanningTree.h"
#include "SquareMatrix.h"
#include <vector>

class UndirectedGraph {
private:
    SquareMatrix * adjacencyMatrix;
public:
    UndirectedGraph(int size);
    UndirectedGraph(SquareMatrix * matrix);
    ~UndirectedGraph();
    void addEdge(int vertex1, int vertex2);
    const int vertexCount();
    bool areConnected(int vertex1, int vertex2);
    std::vector<Edge> * edgesAdjacentTo(int vertex);
    std::vector<Edge> * edgeCandidates(SpanningTree * tree);
    friend std::ostream & operator <<(std::ostream & os, const UndirectedGraph & graph);
    bool getMatrixElem(int i);
};

#endif	/* UNDIRECTEDGRAPH_H */

