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

void path::add(int32_t x, int32_t y){
	waypoint_t point = {x,y};
	L.push_back(point);
}

waypoint_t path::next(waypoint_t pos){
	if (L.size() ==0) return pos;

	int32_t dx = L.front().x - pos.x;
	int32_t dy = L.front().y - pos.y;
	if (dx*dx + dy*dy < int32_t(dist*dist)) L.pop_front();
	return L.front();
}

waypoint_t path::get_directions(int32_t x, int32_t y){
	waypoint_t point = {x,y};
	return get_directions(point);
}

waypoint_t path::get_directions(waypoint_t pos){
	waypoint_t dir = {0};

	if (L.size() ==0) return dir;

	int32_t dx = L.front().x - pos.x;
	int32_t dy = L.front().y - pos.y;
	if (dx*dx + dy*dy < int32_t(dist*dist)) L.pop_front();

	dir.x = dx > 0 ? 1 : -1;
	dir.y = dy > 0 ? 1 : -1;

	return dir;
}
