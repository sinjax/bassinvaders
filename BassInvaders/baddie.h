/*
 * Baddie.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef BADDIE_H_
#define BADDIE_H_

#include "Renderable.h"

/* JG TODO:
 * - type
 * - encapsulates sprite(s)
 */
class Baddie: public Renderable {
public:
	Baddie();
	virtual ~Baddie();
};

#endif /* BADDIE_H_ */
