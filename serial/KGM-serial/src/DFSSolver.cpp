/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	this.graph = graph;
	
	stack = new stack<Edge>();
	bestPrice = 0;
	best = 0;
	
	int vertices = graph.vertexCount();
	spanningTree = std::vector<Edge>(vertices);
	vertexDegrees = new int[vertices];
}

DFSSolver::~DFSSolver() {
	delete graph;
	delete stack;
	delete [] vertexDegrees;
}

void DFSSolver::addEdge(Edge edge) {
	spanningTree.push_back(edge);
	vertexDegrees[edge.vertex1]++;
	vertexDegrees[edge.vertex2]++;
}

void DFSSolver::removeLastEdge() {
	Edge removed = spanningTree.pop_back();
	vertexDegrees[removed.vertex1]--;
	vertexDegrees[removed.vertex2]--;	
}

void DFSSolver::pushBacktrackMarker() {
	stack->push(Edge(-1, -1));
}

std::vector<Edge> DFSSolver::firstEdgeCandidates() {
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