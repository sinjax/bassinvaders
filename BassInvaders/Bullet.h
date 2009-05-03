/*
 * Bullet.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Renderable.h"

/* JG TODO:
 * - type (ie kills which goon)
 * - encapsulates sprite
 */
class Bullet: public Renderable {
public:
	Bullet();
	virtual ~Bullet();
};

#endif /* BULLET_H_ */
