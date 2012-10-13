/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	graph = graph;
	
	edgeStack = new std::stack<Edge>();
	bestPrice = 0;
	best = 0;
		
	spanningTree = std::vector<Edge>();
	int vertices = graph->vertexCount();
	vertexDegrees = new int[vertices];
}

DFSSolver::~DFSSolver() {
	delete graph;
	delete edgeStack;	
	delete [] vertexDegrees;
	delete best;
}

void DFSSolver::addEdge(Edge edge) {
	spanningTree.push_back(edge);
	vertexDegrees[edge.vertex1]++;
	vertexDegrees[edge.vertex2]++;
}

void DFSSolver::removeLastEdge() {
	Edge removed = spanningTree.back();
	vertexDegrees[removed.vertex1]--;
	vertexDegrees[removed.vertex2]--;
	spanningTree.pop_back();
}

void DFSSolver::pushBacktrackMarker() {
	edgeStack->push(Edge(-1, -1));
}

std::vector<Edge> * DFSSolver::firstEdgeCandidates() {
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
	if (best == 0) { // == null
		return true;
	}
	return price < bestPrice;
}

void DFSSolver::updateBest(int price) {
	bestPrice = price;	
	best = new std::vector<Edge>(spanningTree); // make a copy
}

bool DFSSolver::solutionExists() {
	return best != 0; // != null
}

void DFSSolver::printSpanningTree(int price) {
	if (isSolution()) {
			std::cout << "spanning tree:" << std::endl;
		}
		for (int i = 0; i < spanningTree.size(); i++) {
			if (i != 0) {
				std::cout << ", ";
			}
			Edge edge = spanningTree[i];
			std::cout << edge << std::endl;
		}
		std::cout << std::endl << price << std::endl << std::endl;
}

void DFSSolver::printStack() {
	std::cout << "TODO: print stack" << std::endl;
}

void DFSSolver::printCandidates(std::vector<Edge> * candidates) {
	std::cout << candidates->size() << " possibilities: ";
	for (int i = 0; i < candidates->size(); i++) {
		Edge & edge = candidates->at(i);
		std::cout << edge << " ";
	}
	std::cout << std::endl;
}

void DFSSolver::printVertexDegrees(int * degrees) {
	std::cout << "| "; 
	for (int i = 0; i < graph->vertexCount(); i++) {
		std::cout << degrees[i] << " | ";
	}
	std::cout << std::endl;
}