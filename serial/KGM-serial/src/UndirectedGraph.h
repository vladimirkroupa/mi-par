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
	void addEdge(int vertex1, int vertex2);
	const int vertexCount();
	bool areConnected(int vertex1, int vertex2);	
	std::vector<Edge> * edgesAdjacentTo(int vertex);
	std::vector<Edge> * edgeCandidates(std::vector<Edge> & tree, int vertexDegrees[]);
	friend std::ostream & operator <<(std::ostream & os, const UndirectedGraph & graph);
private:
	int matrixSize;
	bool ** adjacencyMatrix;
	bool ** initMatrix(int size);
	bool ** copyMatrix(bool ** original, int size);
};

#endif	/* UNDIRECTEDGRAPH_H */

