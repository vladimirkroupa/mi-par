/* 
 * File:   UndirectedGraph.cpp
 * 
 */

#include "UndirectedGraph.h"

bool** initMatrix(int size) {
	bool** matrix = new bool* [size];
	for (int i = 0; i < size; i++) {
		matrix[i] = new bool[size];
		for (int j = 0; j < size; j++) {
			matrix[i][j] = false;
		}
	}
	return matrix;
}

bool** copyMatrix(bool ** original, int size) {
	bool** copy = initMatrix(size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			copy[i][j] = original[i][j];
		}
	}
	return copy;
}

UndirectedGraph::UndirectedGraph(int size) {
	this->matrixSize = size;
	this->adjacencyMatrix = initMatrix(size);
}

UndirectedGraph::~UndirectedGraph() {
	for (int i = 0; i < matrixSize; i++) {
		delete [] adjacencyMatrix[i];
	}
	delete [] adjacencyMatrix;
}

const int UndirectedGraph::vertexCount() {
	return this->matrixSize;
}

bool UndirectedGraph::areConnected(int vertexIndex1, int vertexIndex2) {
	return this->adjacencyMatrix[vertexIndex1][vertexIndex2];
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

std::vector<Edge> * UndirectedGraph::edgeCandidates(const std::vector<Edge> & tree, int vertexDegrees[]) {
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

	return newEdges;
}

