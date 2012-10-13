/* 
 * File:   DFSSolver.h
 */

#ifndef DFSSOLVER_H
#define	DFSSOLVER_H

#include "UndirectedGraph.h"

class DFSSolver {
public:
	DFSSolver();
	virtual ~DFSSolver();
	std::vector<Edge> * findBestSolution();
private:
	const int MIN_PRICE_POSSIBLE = 2;
	stack<Edge> stack;
	UndirectedGraph graph;
	
	int* vertexDegrees;
	std::vector<Edge> spanningTree;
	
	std::vector<Edge> best;
	int bestPrice;
	
	void addEdge(Edge edge);
	void removeLastEdge();
	
	void pushBacktrackMarker();
	std::vector<Edge> * firstEdgeCandidates();
	
	int evaluate();
	int evaluate(Edge current);
	
	bool possibleWinner(Edge current);
	bool isSolution();
	bool isBestPossible(int price);
	bool isBestSoFar(int price);
	void updateBest(int price);
	bool solutionExists();
	
	void printSpanningTree(int price);
	void printStack();
	void printCandidates(std::vector<Edge> * candidates);
	void printVertexDegrees(int* degrees);
};

#endif	/* DFSSOLVER_H */

