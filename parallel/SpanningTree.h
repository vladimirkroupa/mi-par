/* 
 * File:   SpanningTree.h
 */

#ifndef SPANNINGTREE_H
#define	SPANNINGTREE_H

#include "Edge.h"
#include <vector>

class SpanningTree {
public:
	SpanningTree(int size);
	SpanningTree(int vertexCount, int edgeCount, int * degrees, int * edges);
	SpanningTree(const SpanningTree& orig);
	virtual ~SpanningTree();
	void addEdge(Edge edge);
	void removeLastEdge();
	int edgeCount() const;
	int evaluate() const;
	int evaluate(Edge current) const;
	int * getVertexDegrees() const;
	int vertexCount() const;
	std::vector<Edge> * getEdges() const;
	friend std::ostream & operator <<(std::ostream & os, const SpanningTree & tree);
	void printVertexDegrees();
private:
	int vertexCnt;
	int * vertexDegrees;
	std::vector<Edge> * edges;
};

#endif	/* SPANNINGTREE_H */

