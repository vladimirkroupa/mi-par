/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"

using namespace std;

DFSSolver::DFSSolver(UndirectedGraph * _graph) {
	graph = _graph;
	
	edgeStack = new stack<Edge>();
	bestPrice = 0;
	best = NULL;
	
	int vertices = graph->vertexCount();	
	spanningTree = new vector<Edge>();
	vertexDegrees = new int[vertices];
	for (int i = 0; i < vertices; i++) {
		vertexDegrees[i] = 0;
	}
}

DFSSolver::~DFSSolver() {
	delete edgeStack;	
	delete spanningTree;
	delete [] vertexDegrees;
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
			removeLastEdge();
		} else {
			// add current edge to spanning tree, increment vertex degrees
			addEdge(current);
			if (DEBUG) printVertexDegrees();
			int price = evaluate();
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
				removeLastEdge();
			} else {
				// add backtrack marker to stack so we will know when we are moving up the DFS tree
				pushBacktrackMarker();
				// find new edges to add to spanning tree
				vector<Edge> * candidates = graph->edgeCandidates(*spanningTree, vertexDegrees);
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

void DFSSolver::addEdge(Edge edge) {	
	vertexDegrees[edge.vertex1]++;
	vertexDegrees[edge.vertex2]++;
	spanningTree->push_back(edge);
}

void DFSSolver::removeLastEdge() {
	Edge removed = spanningTree->back();
	vertexDegrees[removed.vertex1]--;
	vertexDegrees[removed.vertex2]--;
	spanningTree->pop_back();
}

void DFSSolver::pushBacktrackMarker() {
	edgeStack->push(Edge(-1, -1));
}

vector<Edge> * DFSSolver::firstEdgeCandidates() {
	int vertex = 0;
	return graph->edgesAdjacentTo(vertex);
}

int DFSSolver::evaluate() {
	int max = vertexDegrees[0];
	for (int i = 0; i < graph->vertexCount(); i++) {
		if (vertexDegrees[i] > max) {
			max = vertexDegrees[i];
		}
	}
	return max;
}

int DFSSolver::evaluate(Edge current) {
	int max = vertexDegrees[0];
	for (int i = 0; i < graph->vertexCount(); i++) {
		int vertexDeg = vertexDegrees[i]; 
		if (current.vertex1 == i || current.vertex2 == i) {
			vertexDeg++;
		}
		if (vertexDeg > max) {
			max = vertexDeg;
		}
	}
	return max;	
}

bool DFSSolver::possibleWinner(Edge current) {
	int price = evaluate(current);
	return isBestSoFar(price);
}

bool DFSSolver::isSolution() {
	int vertices = graph->vertexCount();
	int degreeSum = 0;
	for (int i = 0; i < vertices; i++) {
		degreeSum += vertexDegrees[i];
	}	
	int edges = degreeSum / 2;
	
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
	best = new vector<Edge>(*spanningTree); // make a copy
}

bool DFSSolver::solutionExists() {
	return best != NULL;
}

void DFSSolver::printSpanningTree(int price) {
	if (isSolution()) {
		cout << "spanning tree, degree: " << price << endl;
	}
	for (int i = 0; i < spanningTree->size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		Edge & edge = (*spanningTree)[i];
		cout << edge;
	}
	cout << endl << endl;
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
	cout << "| "; 
	for (int i = 0; i < graph->vertexCount(); i++) {
		cout << vertexDegrees[i] << " | ";
	}
	cout << endl;
}

pair<vector<Edge> *, int> * DFSSolver::prepareSolution(vector<Edge>* solution, int solutionPrice) {
	vector<Edge> * copy = new vector<Edge>(*solution);
	pair<vector<Edge> *, int> * result = new pair<vector<Edge> *, int>(copy, solutionPrice);
	return result;
}
