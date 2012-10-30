/* 
 * File:   UndirectedGraph.h
 * 
 */

#ifndef UNDIRECTEDGRAPH_H
#define	UNDIRECTEDGRAPH_H

#include "Edge.h"
#include "SquareMatrix.h"
#include <vector>

class UndirectedGraph {
private:
    SquareMatrix * adjacencyMatrix;
public:
    UndirectedGraph(int size);
    ~UndirectedGraph();
    void addEdge(int vertex1, int vertex2);
    const int vertexCount();
    bool areConnected(int vertex1, int vertex2);
    std::vector<Edge> * edgesAdjacentTo(int vertex);
    std::vector<Edge> * edgeCandidates(std::vector<Edge> & tree, int vertexDegrees[]);
    friend std::ostream & operator <<(std::ostream & os, const UndirectedGraph & graph);
};

#endif	/* UNDIRECTEDGRAPH_H */

