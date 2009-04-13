/*
 * path.h
 *
 *  Created on: Apr 13, 2009
 *      Author: Darren Golbourn
 */

#ifndef PATH_H_
#define PATH_H_

#include <inttypes.h>
#include <list>

typedef struct waypoint{
	int32_t x;
	int32_t y;
}waypoint_t;

class path {
	std::list<waypoint_t> L;
	uint32_t dist;
public:
	path(uint32_t dist);
	virtual ~path();
	void add(waypoint_t pos);
	waypoint_t next(waypoint_t pos);
};

#endif /* PATH_H_ */
