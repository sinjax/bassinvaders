/*
 * Formation.h
 *
 *  Created on: May 24, 2009
 *      Author: Darren Golbourn
 *
 *      Description:	This abstract class is just a stub for enemy formations.
 *      				A formation is described as a bunch of Entity entites
 *      				and a little bit of game logic to make them "fun".
 *
 */

#ifndef FORMATION_H_
#define FORMATION_H_

#include "Entity.h"
#include <list>

class Formation {
public:
	Formation();
	virtual ~Formation() = 0;
	std::list<Entity*> entities;
};

#endif /* FORMATION_H_ */
