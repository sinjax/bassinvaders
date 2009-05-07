/*
 * InputManager.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "InputManager.h"
#include "toolkit.h"

InputManager::InputManager()
{
	/* set up the default key mappings */
	memset(keyMap, 0, (sizeof(uint32_t) * (MAX_KEYS)));
	keyMap[SDLK_DOWN] = ACTION_MOVE_DOWN;
	keyMap[SDLK_UP] = ACTION_MOVE_UP;
	keyMap[SDLK_LEFT] = ACTION_MOVE_LEFT;
	keyMap[SDLK_RIGHT] = ACTION_MOVE_RIGHT;
	keyMap[SDLK_SPACE] = ACTION_SHOOT;
	keyMap[SDLK_ESCAPE] = ACTION_QUIT;
	keyMap[SDLK_v] = ACTION_DIE;
	keyMap[SDLK_b] = ACTION_LIVE;

}

InputManager::~InputManager() {

}
/* change the action(s) associated with a key*/
void InputManager::mapKeyToAction(uint32_t action, SDLKey key)
{
	if (key < MAX_KEYS)
	{
		keyMap[key] = action;
	}
}

uint32_t InputManager::getKeyAction(SDLKey key)
{
	if (key < MAX_KEYS)
	{
		return keyMap[key];
	}
	else
	{
		DebugPrint(("%s: Shouldn't get here\n", "InputManager::getKeyAction"));
		return ACTION_INVALID;
	}
}

/* returns a bitmaks of all the actions that are currently
 * being requested by the user */
uint32_t InputManager::getCurrentActions()
{
	uint32_t actions = 0;
	uint8_t* keys = SDL_GetKeyState(NULL);

	for (uint32_t i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i])
		{
			actions |= getKeyAction( (SDLKey) i);
		}
	}


	return actions;
}
