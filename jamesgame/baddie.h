/*
 * baddie.h
 *
 *  Created on: Apr 13, 2009
 *      Author: Darren Golbourn
 */

#ifndef BADDIE_H_
#define BADDIE_H_

#include "sprite.h"
#include "path.h"

class baddie: public sprite {
public:
	baddie(game*);
	virtual ~baddie();
	void moveSprite();
	Uint32 height;
	Uint32 width;
};

#endif /* BADDIE_H_ */
