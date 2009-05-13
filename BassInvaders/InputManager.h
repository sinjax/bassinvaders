/*
 * InputManager.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 *
 *  fairly minimal class
 *  just provides a simple way of mapping player input to
 *  the actions that can happen.
 *  The main game loop is still expected to call GetKeys and poll for key events.
 */

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "SDL.h"

#define MAX_KEYS (SDLK_LAST-1)

#define ACTION_INVALID        0x00000000
#define ACTION_MOVE_DOWN      0x00000001
#define ACTION_MOVE_UP        0x00000002
#define ACTION_MOVE_LEFT      0x00000004
#define ACTION_MOVE_RIGHT     0x00000008
#define ACTION_SHOOT          0x00000010
#define ACTION_QUIT           0x00000020
#define ACTION_DIE            0x00000040
#define ACTION_LIVE            0x00000080

#define ACTIONMASK uint32_t

class InputManager {
public:
	InputManager();
	~InputManager();

	void mapKeyToAction(ACTIONMASK action, SDLKey key);
	ACTIONMASK getKeyAction(SDLKey key);
	ACTIONMASK getCurrentActions();


private:
	uint32_t keyMap[MAX_KEYS];
	/* a look up table of keys to actions.
	 * indexed on sdl key so you can have multiple keys bound to the same action
	 */
};

#endif /* INPUTMANAGER_H_ */
