/*
 * game.h
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman, Darren Golbourn
 */

#ifndef game_H_
#define game_H_

#include "includes.h"
#include <list>
#include <map>
#include <string>
#include "SDL_mixer.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32
#define CHUNK 4096 					//size of audio stream chunks
#define INSERT_YOUR_SONG_PATH_HERE "sound/test.mp3"

class game
{
public:
	game();
	~game();

	SDL_Surface* getWindowSurface();
	Mix_Music *music;
	void Flip();
	bool shouldIQuit();
	void gameloop();
	Uint8* keys;
	void * getResource(std::string s);
	std::list<sprite*> sprite_list;

private:
	SDL_Surface* pScreen;
	background *bg;
	std::map<std::string, void*> resources;
};

SDL_Surface* LoadImage(char* filename);
void DrawToSurface( Uint32 x, Uint32 y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip = NULL);


#endif /* game_H_ */
