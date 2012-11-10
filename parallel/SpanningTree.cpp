/* 
 * File:   SpanningTree.cpp
 */

#include "SpanningTree.h"

using namespace std;

SpanningTree::SpanningTree(int vertexCount) {
	this->vertexCnt = vertexCount;
	this->edges = new vector<Edge>();
	this->vertexDegrees = new int[vertexCount];
	std::fill_n(vertexDegrees, vertexCount, 0);
}

SpanningTree::SpanningTree(int vertexCount, int edgeCount, int * degrees, int * edges) {
	this->vertexCnt = vertexCount;
	this->vertexDegrees = degrees;
	this->edges = new vector<Edge>();
	for (int i = 0; i < edgeCount * 2; i++) {
		Edge e = Edge(edges[i], edges[i + 1]);
		this->edges->insert(this->edges->end(), e);
		i++;
	}
}

SpanningTree::SpanningTree(const SpanningTree& orig) {
	this->vertexCnt = orig.vertexCnt;
	this->edges = new vector<Edge>(*orig.edges);
	this->vertexDegrees = new int[vertexCnt];
	std::copy(orig.vertexDegrees, orig.vertexDegrees + vertexCnt, vertexDegrees);	
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
	for (int i = 0; i < vertexCnt; i++) {
		if (vertexDegrees[i] > max) {
			max = vertexDegrees[i];
		}
	}
	return max;
}

int SpanningTree::evaluate(Edge current) const {
	int max = vertexDegrees[0];
	for (int i = 0; i < vertexCnt; i++) {
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

int SpanningTree::vertexCount() const {
	return vertexCnt;
}

vector<Edge> * SpanningTree::getEdges() const {
	return edges;
}

std::ostream & operator <<(std::ostream & os, const SpanningTree & tree) {
	for (unsigned i = 0; i < tree.edges->size(); i++) {
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
	for (int i = 0; i < vertexCnt; i++) {
		cout << vertexDegrees[i] << " | ";
	}
	cout << endl;
}
