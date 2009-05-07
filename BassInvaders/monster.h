/*
 * monster.h
 *
 *  Created on: 8/5/09
 *      Author: Darren Golbourn
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "Renderable.h"
#include <vector>
#include "InputManager.h"
#include "ResourceBundle.h"

#define MONSTER_X_SPEED 5
#define MONSTER_Y_SPEED 0

class monster: public Renderable {
public:
	monster();
	virtual ~monster();

	virtual bool isCollidingWith(Renderable *pRenderable);
	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);

private:
	void loadMonsterData();
	uint32_t velocityTicks;
	uint32_t lastTickCount;
};

#endif /* HERO_H_ */
