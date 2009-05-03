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
#include "InputManager.h"
//i am testing git gui
#define BODYSPRITE 0 //main body of hero is the first sprite in the vector

#define HERO_X_SPEED 5
#define HERO_Y_SPEED 5
/* JG TODO:
 * - Vector of bullets
 */

class Hero: public Renderable {
public:
	Hero(const char* filename);
	virtual ~Hero();

	virtual bool isCollidingWith(Renderable *pRenderable);
	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);

	void setActions(ACTIONMASK actions);

private:
	void loadHeroData(FILE *fp);
	void doActions();

private:
	std::vector<Bullet> bullets;
	uint32_t velocityTicks; //how many ticks pass before we move by velocity
	uint32_t lastTickCount;
};

#endif /* HERO_H_ */
