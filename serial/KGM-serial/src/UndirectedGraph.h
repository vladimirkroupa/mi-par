/* 
 * File:   UndirectedGraph.h
 * 
 */

#ifndef UNDIRECTEDGRAPH_H
#define	UNDIRECTEDGRAPH_H

#include "Edge.h"
#include <vector>

class UndirectedGraph {
public:
	UndirectedGraph(int size);
	~UndirectedGraph();
	const int vertexCount();
	bool areConnected(int vertexIndex1, int vertexIndex2);	
	std::vector<Edge> * edgesAdjacentTo(int vertex);
	std::vector<Edge> * edgeCandidates(const std::vector<Edge> & tree, int vertexDegrees[]);
private:
	int matrixSize;
	bool** adjacencyMatrix;
};

#endif	/* UNDIRECTEDGRAPH_H */

