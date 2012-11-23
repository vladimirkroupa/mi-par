/* 
 * File:   TimeTool.h
 *
 */

#ifndef TIME_TOOL_H
#define	TIME_TOOL_H

#include <iostream>
#include <time.h>

class TimeTool {
public:
	time_t time;
	TimeTool(time_t);
	friend std::ostream & operator <<(std::ostream & os, const TimeTool & tt);
private:

};

#endif	/* TIME_TOOLS_H */

