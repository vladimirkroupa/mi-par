/* 
 * File:   DFSSolver.h
 */

#ifndef DFSSOLVER_H
#define	DFSSOLVER_H

#include "UndirectedGraph.h"
#include "SpanningTree.h"
#include "Token.h"
#include <stack>
#include <utility>
#include <string>
#include "mpi.h"

#define WORK_REQ 1
#define WORK_SHARE 2
#define WORK_REJECT 3
#define TOKEN 4
#define TERMINATE 5


class DFSSolver {
public:
	DFSSolver(UndirectedGraph * graph);
	virtual ~DFSSolver();
	std::pair<std::vector<Edge> *, int> * findBestSolution();
private:
	static const int MIN_PRICE_POSSIBLE = 2;
	static const bool DEBUG = false;
	
	std::vector<Edge> * edgeStack;
	UndirectedGraph * graph;
	
	SpanningTree * spanningTree;
	
	SpanningTree * best;
	int bestPrice;
	
	int rank;
	int commSize;
	MPI_Comm comm;

	bool finished;
	Token color;
	bool whiteTokenSent;
	int workCounter;

	void pushBacktrackMarker();
	std::vector<Edge> * firstEdgeCandidates();

	bool possibleWinner(Edge current);
	bool isSolution();
	bool isBestPossible(int price);
	bool isBestSoFar(int price);
	void updateBest(int price);
	bool solutionExists();
	
	void printSpanningTree(SpanningTree * tree);
	void printStack(std::vector<Edge> * stack);
	void printCandidates(std::vector<Edge> * candidates);
	void printVertexDegrees();
	
	std::pair<std::vector<Edge> *, int> * prepareSolution(SpanningTree * solution, int solutionPrice);
	
	std::pair<std::vector<Edge> *, SpanningTree *> * splitWork();
	bool shouldTerminate();
	bool hasWorkToShare();
	void handleWorkRequests();
	void sendWork(int to);
	void rejectAll();
	void sendWorkRequest();
	bool checkWorkResponse(bool * workRequestSent, int * availableFrom);
	void receiveWork(int source);
	void checkColorChange(int sentWorkTo);
	void askToTerminate();
	void checkTerminationMsg();
	void handleTokens();

	void initialTokenSend();
	void sendToken(Token token);
    void receiveToken();
	int prevNode();
	int nextNode();
};

#endif	/* DFSSOLVER_H */

