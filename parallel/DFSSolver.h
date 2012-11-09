/* 
 * File:   DFSSolver.h
 */

#ifndef DFSSOLVER_H
#define	DFSSOLVER_H

#include "UndirectedGraph.h"
#include "SpanningTree.h"
#include <stack>
#include <utility>

class DFSSolver {
public:
	DFSSolver(UndirectedGraph * graph);
	virtual ~DFSSolver();
	std::pair<std::vector<Edge> *, int> * findBestSolution();
private:
	static const int MIN_PRICE_POSSIBLE = 2;
	static const bool DEBUG = false;
	std::stack<Edge> * edgeStack;
	UndirectedGraph * graph;
	
	SpanningTree * spanningTree;
	
	SpanningTree * best;
	int bestPrice;
	
	void pushBacktrackMarker();
	std::vector<Edge> * firstEdgeCandidates();

	bool possibleWinner(Edge current);
	bool isSolution();
	bool isBestPossible(int price);
	bool isBestSoFar(int price);
	void updateBest(int price);
	bool solutionExists();
	
	void printSpanningTree(int price);
	void printStack();
	void printCandidates(std::vector<Edge> * candidates);
	void printVertexDegrees();
	
	std::pair<std::vector<Edge> *, int> * prepareSolution(SpanningTree * solution, int solutionPrice);
};

#endif	/* DFSSOLVER_H */

