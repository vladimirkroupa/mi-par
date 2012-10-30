/* 
 * File:   SpanningTree.cpp
 */

#include "SpanningTree.h"

using namespace std;

SpanningTree::SpanningTree(int vertexCount) {
	this->vertexCount = vertexCount;
	edges = new vector<Edge>();
	vertexDegrees = new int[vertexCount];
	std::fill_n(vertexDegrees, vertexCount, 0);
}

SpanningTree::SpanningTree(const SpanningTree& orig) {
	this->vertexCount = orig.vertexCount;
	edges = new vector<Edge>(*orig.edges);
	vertexDegrees = new int[vertexCount];
	std::copy(orig.vertexDegrees, orig.vertexDegrees + vertexCount, vertexDegrees);	
}

SpanningTree::~SpanningTree() {
	delete edges;
	delete [] vertexDegrees;
}

void SpanningTree::addEdge(Edge edge) {
	vertexDegrees[edge.vertex1]++;
	vertexDegrees[edge.vertex2]++;
	edges->push_back(edge);
}

void SpanningTree::removeLastEdge() {
	Edge removed = edges->back();
	vertexDegrees[removed.vertex1]--;
	vertexDegrees[removed.vertex2]--;
	edges->pop_back();
}

int SpanningTree::edgeCount() const {
	return edges->size();
}

int SpanningTree::evaluate() const {
	int max = vertexDegrees[0];
	for (int i = 0; i < vertexCount; i++) {
		if (vertexDegrees[i] > max) {
			max = vertexDegrees[i];
		}
	}
	return max;
}

int SpanningTree::evaluate(Edge current) const {
	int max = vertexDegrees[0];
	for (int i = 0; i < vertexCount; i++) {
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

int * SpanningTree::getVertexDegrees() const {
	return vertexDegrees;
}

vector<Edge> * SpanningTree::getEdges() const {
	return edges;
}

std::ostream & operator <<(std::ostream & os, const SpanningTree & tree) {
	for (int i = 0; i < tree.edges->size(); i++) {
		if (i != 0) {
			os << ", ";
		}
		Edge & edge = (*tree.edges)[i];
		os << edge;
	}
	os << endl << "degree: " << tree.evaluate() << endl;
	return os;
}

void SpanningTree::printVertexDegrees() {
	cout << "| "; 
	for (int i = 0; i < vertexCount; i++) {
		cout << vertexDegrees[i] << " | ";
	}
	cout << endl;
}