/* 
 * File:   TimeTool.cpp
 * 
 */

#include <iostream>

#include "TimeTool.h"

TimeTool::TimeTool(time_t time) {
	this->time = time;
}

std::ostream & operator <<(std::ostream & os, const TimeTool & tt) {
	int h = tt.time/3600;
	int m = tt.time /60;
	int s = tt.time - (h * 3600) - (m * 60);

	os << h << ":" << m << ":" << s;
	return os;
}


