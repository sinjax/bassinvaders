/*
 * Scene.h
 *
 *  Created on: May 24, 2009
 *      Author: Darren Golbourn
 *
 *      Description:	A scene is a collection of formations, on screen text, events
 *      				and game logic that together make a themed set piece.
 *      				A level is made up of scenes, one scene at a time.
 */

#ifndef SCENE_H_
#define SCENE_H_
#include "Formation.h"
#include <list>

class Scene {
public:
	Scene();
	virtual ~Scene() = 0;
	std::list<Formation*> formations;
};

#endif /* SCENE_H_ */
