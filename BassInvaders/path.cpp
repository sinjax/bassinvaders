/*
 * path.cpp
 *
 *  Created on: Apr 13, 2009
 *      Author: Darren Golbourn
 */

#include "path.h"

path::path(uint32_t dist) {
	this->dist = dist;
}

path::~path() {
}

void path::add(waypoint_t pos){
	L.push_back(pos);
}

waypoint_t path::next(waypoint_t pos){
	if (L.size() ==0) return pos;

	uint32_t dx = L.front().x - pos.x;
	uint32_t dy = L.front().y - pos.y;
	if (dx*dx + dy*dy < dist*dist) L.pop_front();
	return L.front();
}
