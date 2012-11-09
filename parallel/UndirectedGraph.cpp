/* 
 * File:   UndirectedGraph.cpp
 * 
 */

#include "UndirectedGraph.h"

using namespace std;

UndirectedGraph::UndirectedGraph(int size) {
	this->adjacencyMatrix = new SquareMatrix(size);
}

UndirectedGraph::~UndirectedGraph() {
	delete adjacencyMatrix;
}

void UndirectedGraph::addEdge(int vertex1, int vertex2) {
	(*adjacencyMatrix)(vertex1, vertex2) = true;
	// to be sure, but should not be needed
	(*adjacencyMatrix)(vertex2, vertex1) = true;
}

const int UndirectedGraph::vertexCount() {
	return adjacencyMatrix->size();
}

bool UndirectedGraph::areConnected(int vertex1, int vertex2) {
	return (*adjacencyMatrix)(vertex1, vertex2);
}

std::vector<Edge> * UndirectedGraph::edgesAdjacentTo(int vertex) {
	std::vector<Edge> * edges = new std::vector<Edge>();
	for (int vertex2 = 0; vertex2 < vertexCount(); vertex2++) {
		if (areConnected(vertex, vertex2)) {
			edges->push_back(Edge(vertex, vertex2));
		}
	}
	return edges;
}

/**
 * Finds edges of this graph such that one vertex of the edge is contained in 
 * given tree and the other vertex is not.
 * 
 * @param tree
 * @param vertexDegrees degrees of all vertices of the tree
 * @return vector of edges satisfying the conditions
 */
std::vector<Edge> * UndirectedGraph::edgeCandidates(SpanningTree * tree) {
	vector<Edge> * edges = tree->getEdges();
	int * vertexDegrees = tree->getVertexDegrees();
	SquareMatrix * adjacencyCopy = new SquareMatrix(*adjacencyMatrix);
	// remove edges already present in tree from adjacency matrix
	for (int i = 0; i < edges->size(); i++) {
		Edge edge = (*edges)[i];
		(*adjacencyCopy)(edge.vertex1, edge.vertex2) = false;
	}

	std::vector<Edge> * newEdges = new std::vector<Edge>();
	for (int vertexFrom = 0; vertexFrom < vertexCount(); vertexFrom++) {
		// skip edges where the first vertex would have degree 0 in the tree
		if (vertexDegrees[vertexFrom] == 0) {
			continue;
		}
		for (int vertexTo = 0; vertexTo < vertexCount(); vertexTo++) {
			// skip edges that would create cycle in the tree
			if (vertexDegrees[vertexTo] != 0) {
				continue;
			}
			if ((*adjacencyCopy)(vertexFrom, vertexTo) == true) {
				newEdges->push_back(Edge(vertexFrom, vertexTo));
			}
		}
	}

	delete adjacencyCopy;
	
	return newEdges;
}

std::ostream & operator <<(std::ostream & os, const UndirectedGraph & graph) {
	os << " | ";
	for (int i = 0; i < graph.adjacencyMatrix->size(); i++) {
		os << i << " ";
	}
	os << std::endl;
	
	os << "---";
	for (int i = 0; i < graph.adjacencyMatrix->size(); i++) {
		 os << "--";
	}
	os << std::endl;
	
	for (int i = 0; i < graph.adjacencyMatrix->size(); i++) {
		os << i << "| ";
		for (int j = 0; j < graph.adjacencyMatrix->size(); j++) {
			os << (*graph.adjacencyMatrix)(i, j) << " ";
		}
		os << std::endl;
	}
	os << std::endl;
	return os;
}
