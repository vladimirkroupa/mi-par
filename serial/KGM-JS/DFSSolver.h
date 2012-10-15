/* 
 * File:   DFSSolver.h
 */

#ifndef DFSSOLVER_H
#define	DFSSOLVER_H

#include "UndirectedGraph.h"
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
	
	int * vertexDegrees;
	std::vector<Edge> * spanningTree;
	
	std::vector<Edge> * best;
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
	void printVertexDegrees();
	
	std::pair<std::vector<Edge> *, int> * prepareSolution(std::vector<Edge> * solution, int solutionPrice);
};

#endif	/* DFSSOLVER_H */

