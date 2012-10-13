/* 
 * File:   Edge.h
 *
 */

#ifndef EDGE_H
#define	EDGE_H

#include <iostream>

class Edge {
public:
	int vertex1;
	int vertex2;
	Edge(int vertex1, int vertex2);
	virtual ~Edge();
	bool isBacktrackMarker();
	friend std::ostream & operator <<(std::ostream & os, const Edge & e);
private:

};

#endif	/* EDGE_H */