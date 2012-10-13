/* 
 * File:   BacktrackMarker.h
 */

#ifndef BACKTRACKMARKER_H
#define	BACKTRACKMARKER_H

#include "Edge.h"


class BacktrackMarker : Edge {
public:
	BacktrackMarker();
	virtual ~BacktrackMarker();
	bool isBacktrackMarker();
};

#endif	/* BACKTRACKMARKER_H */

