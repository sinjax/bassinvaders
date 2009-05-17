/*
 * bomb.h
 *
 *  Created on: 12/5/09
 *      Author: Darren Golbourn
 */

#ifndef BOMB_H_
#define BOMB_H_

#include "Renderable.h"
#include <vector>
#include "InputManager.h"
#include "ResourceBundle.h"

#define BOMB_X_SPEED -1
#define BOMB_Y_SPEED 0

class bomb: public Renderable {
public:
	bomb(uint32_t);
	virtual ~bomb();

	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);
	void collide(Renderable*);
private:
	void loadBombData();
	uint32_t velocityTicks;
	uint32_t lastTickCount;
};

#endif /* BOMB_H_ */
