/* 
 * File:   Edge.cpp
 * 
 */

#include <iostream>

#include "Edge.h"

Edge::Edge(int vertex1, int vertex2) {
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
}

Edge::~Edge() {
}

bool Edge::isBacktrackMarker() {
	return (vertex1 == -1 && vertex2 == -1);
}

std::ostream & operator <<(std::ostream & os, const Edge & e) {
	int edgeFrom, edgeTo;
	if (e.vertex1 <= e.vertex2) {
		edgeFrom = e.vertex1;
		edgeTo = e.vertex2;
	} else {
		edgeFrom = e.vertex2;
		edgeTo = e.vertex1;
	}
	os << "(" << edgeFrom << ", " << edgeTo << ")";
	return os;
}


