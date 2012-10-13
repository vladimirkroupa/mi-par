/* 
 * File:   Edge.h
 *
 */

#ifndef EDGE_H
#define	EDGE_H

class Edge {
public:
	int vertex1;
	int vertex2;
	Edge(int vertex1, int vertex2);
	virtual ~Edge();
	bool isBacktrackMarker();
private:

};

#endif	/* EDGE_H */