/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"

using namespace std;

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	this->graph = graph;
	
	edgeStack = new stack<Edge>();
	bestPrice = 0;
	best = NULL;
	
	spanningTree = new SpanningTree(graph->vertexCount());
}

DFSSolver::~DFSSolver() {
	delete edgeStack;	
	delete spanningTree;
	delete best;
}

pair<vector<Edge> *, int> * DFSSolver::findBestSolution() {
	// initial state - push all edges adjacent to vertex 0 to stack
	vector<Edge> * initial = firstEdgeCandidates();
	for (int i = 0; i < initial->size(); i++) {
		edgeStack->push((*initial)[i]);
	}
	delete initial;
	
	while (! edgeStack->empty()) {
		if (DEBUG) printStack();
		// remove top edge from stack
		Edge current = edgeStack->top();
		edgeStack->pop();

		if (current.isBacktrackMarker()) {
			if (DEBUG) cout << "backtracking" << endl;
			// if current edge is backtrack marker, remove last edge from spanning tree and decrement vertex degrees
			spanningTree->removeLastEdge();
		} else {
			// add current edge to spanning tree, increment vertex degrees
			spanningTree->addEdge(current);
			if (DEBUG) printVertexDegrees();
			int price = spanningTree->evaluate();
			if (DEBUG) printSpanningTree(price);
			if (isSolution()) {
				if (! DEBUG) printSpanningTree(price);
				if (isBestPossible(price)) {
					// if the current solution is the best possible, return
					updateBest(price);
					return prepareSolution(best, bestPrice);
				} else if (isBestSoFar(price)) {
					// if not best possible, but better that any solution so far, update best
					updateBest(price);
				}
				// since we've found the solution, we're at the bottom of the DFS tree -> backtracking
				spanningTree->removeLastEdge();
			} else {
				// add backtrack marker to stack so we will know when we are moving up the DFS tree
				pushBacktrackMarker();
				// find new edges to add to spanning tree
				vector<Edge> * candidates = graph->edgeCandidates(spanningTree);
				if (DEBUG) printCandidates(candidates);
				for (int i = 0; i < candidates->size(); i++) {
					Edge & edge = (*candidates)[i];
					if (possibleWinner(edge)) {
						// if the current candidate edge can lead to better solution than the best solution so far,
						// add it to the stack
						edgeStack->push(edge);
					} else {
						if (DEBUG) cout <<  "leaving out edge " << edge << endl;
					}
				}
				delete candidates;
			}
		}
		if (DEBUG) cout <<  "---------------------------" << endl;
	}
	if (solutionExists()) {
		// at this point, we've completed the DFS tree traversal
		return prepareSolution(best, bestPrice);
	}
	return NULL;
}

void DFSSolver::pushBacktrackMarker() {
	edgeStack->push(Edge(-1, -1));
}

vector<Edge> * DFSSolver::firstEdgeCandidates() {
	int vertex = 0;
	return graph->edgesAdjacentTo(vertex);
}

bool DFSSolver::possibleWinner(Edge current) {
	int price = spanningTree->evaluate(current);
	return isBestSoFar(price);
}

bool DFSSolver::isSolution() {
	int vertices = graph->vertexCount();
	int edges = spanningTree->edgeCount();
	return (edges ==  vertices - 1);
}

bool DFSSolver::isBestPossible(int price) {
	return price == MIN_PRICE_POSSIBLE;
}

bool DFSSolver::isBestSoFar(int price) {
	if (best == NULL) {
		return true;
	}
	return price < bestPrice;
}

void DFSSolver::updateBest(int price) {
	bestPrice = price;
	delete best;
	best = new SpanningTree(*spanningTree); // make a copy
}

bool DFSSolver::solutionExists() {
	return best != NULL;
}

void DFSSolver::printSpanningTree(int price) {
	cout << *spanningTree << endl;
}

void DFSSolver::printStack() {
	//cout << "TODO: print stack" << endl;
}

void DFSSolver::printCandidates(vector<Edge> * candidates) {
	cout << candidates->size() << " possibilities: ";
	for (int i = 0; i < candidates->size(); i++) {
		Edge & edge = (*candidates)[i];
		if (i != 0) {
			cout << ", ";
		}
		cout << edge;
	}
	cout << endl;
}

void DFSSolver::printVertexDegrees() {
	spanningTree->printVertexDegrees();
}

pair<vector<Edge> *, int> * DFSSolver::prepareSolution(SpanningTree* solution, int solutionPrice) {
	vector<Edge> * copy = new vector<Edge>(*(solution->getEdges()));
	pair<vector<Edge> *, int> * result = new pair<vector<Edge> *, int>(copy, solutionPrice);
	return result;
}
