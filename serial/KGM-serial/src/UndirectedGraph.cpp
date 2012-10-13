/* 
 * File:   UndirectedGraph.cpp
 * 
 */

#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph(int size) {
	this->matrixSize = size;
	this->adjacencyMatrix = initMatrix(size);
}

UndirectedGraph::~UndirectedGraph() {
	deleteMatrix(adjacencyMatrix, matrixSize);
}

void UndirectedGraph::addEdge(int vertex1, int vertex2) {
	adjacencyMatrix[vertex1][vertex2] = true;
	adjacencyMatrix[vertex2][vertex1] = true;
}

const int UndirectedGraph::vertexCount() {
	return matrixSize;
}

bool UndirectedGraph::areConnected(int vertex1, int vertex2) {
	return adjacencyMatrix[vertex1][vertex2];
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

std::vector<Edge> * UndirectedGraph::edgeCandidates(std::vector<Edge> & tree, int vertexDegrees[]) {
	bool ** adjacencyCopy = copyMatrix(adjacencyMatrix, this->matrixSize);
	for (int i = 0; i < tree.size(); i++) {
		Edge edge = tree[i];
		adjacencyCopy[edge.vertex1][edge.vertex2] = false;
		adjacencyCopy[edge.vertex2][edge.vertex1] = false;
	}

	std::vector<Edge> * newEdges = new std::vector<Edge>();
	for (int vertexFrom = 0; vertexFrom < vertexCount(); vertexFrom++) {
		if (vertexDegrees[vertexFrom] == 0) {
			continue;
		}
		for (int vertexTo = 0; vertexTo < vertexCount(); vertexTo++) {
			if (vertexDegrees[vertexTo] != 0) {
				continue;
			}
			if (adjacencyCopy[vertexFrom][vertexTo] == true) {
				newEdges->push_back(Edge(vertexFrom, vertexTo));
			}
		}
	}

	deleteMatrix(adjacencyCopy, matrixSize);
	
	return newEdges;
}

bool** UndirectedGraph::initMatrix(int size) {
	bool** matrix = new bool* [size];
	for (int i = 0; i < size; i++) {
		matrix[i] = new bool[size];
		for (int j = 0; j < size; j++) {
			matrix[i][j] = false;
		}
	}
	return matrix;
}

bool** UndirectedGraph::copyMatrix(bool ** original, int size) {
	bool** copy = initMatrix(size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			copy[i][j] = original[i][j];
		}
	}
	return copy;
}

void UndirectedGraph::deleteMatrix(bool ** original, int size) {
	for (int i = 0; i < size; i++) {
		delete [] original[i];
	}
	delete original;
}

std::ostream & operator <<(std::ostream & os, const UndirectedGraph & graph) {
	os << " | ";
	for (int i = 0; i < graph.matrixSize; i++) {
		os << i << " ";
	}
	os << std::endl;
	
	os << "---";
	for (int i = 0; i < graph.matrixSize; i++) {
		 os << "--";
	}
	os << std::endl;
	
	for (int i = 0; i < graph.matrixSize; i++) {
		os << i << "| ";
		for (int j = 0; j < graph.matrixSize; j++) {
			os << graph.adjacencyMatrix[i][j] << " ";
		}
		os << std::endl;
	}
	os << std::endl;
	return os;
}