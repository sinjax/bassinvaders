/*
 * Hero.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef HERO_H_
#define HERO_H_

#include "Renderable.h"
#include <vector>
#include "Bullet.h"

/* JG TODO:
 * - Vector of bullets
 * - lives
 */

class Hero: public Renderable {
public:
	Hero();
	virtual ~Hero();

	virtual bool isCollidingWith(Renderable *pRenderable);
	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);

private:
	std::vector<Bullet> bullets;
	uint8_t health;
};

#endif /* HERO_H_ */
