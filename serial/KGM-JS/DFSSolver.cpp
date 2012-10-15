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
	vector<Edge> * initial = firstEdgeCandidates();
	for (int i = 0; i < initial->size(); i++) {
		edgeStack->push((*initial)[i]);
	}
	delete initial;
	
	while (! edgeStack->empty()) {
		printStack();
		Edge current = edgeStack->top();
		edgeStack->pop();

		if (current.isBacktrackMarker()) {
			if (DEBUG) cout << "backtracking" << endl;
			removeLastEdge();
		} else {
			addEdge(current);
			if (DEBUG) printVertexDegrees();
			int price = evaluate();
			if (DEBUG) printSpanningTree(price);
			if (isSolution()) {
				if (isBestPossible(price)) {
					return prepareSolution(best, bestPrice);
				} else if (isBestSoFar(price)) {
					updateBest(price);
				}
				removeLastEdge();
			} else {
				pushBacktrackMarker();
				vector<Edge> * candidates = graph->edgeCandidates(*spanningTree, vertexDegrees);
				if (DEBUG) printCandidates(candidates);
				for (int i = 0; i < candidates->size(); i++) {
					Edge & edge = (*candidates)[i];
					if (possibleWinner(edge)) {
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
		cout << "spanning tree:" << endl;
	}
	for (int i = 0; i < spanningTree->size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		Edge & edge = (*spanningTree)[i];
		cout << edge;
	}
	cout << endl;
	cout << endl << "price: " << price << endl << endl;
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
