/* 
 * File:   Edge.cpp
 * 
 */

#include "Edge.h"

Edge::Edge(int vertex1, int vertex2) {
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
}

Edge::~Edge() {
}

bool Edge::isBacktrackMarker() {
	return false;
}


