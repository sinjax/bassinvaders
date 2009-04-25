/*
 * game.h
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman, Darren Golbourn, Sina Samangooei
 */

#ifndef game_H_
#define game_H_

#include "includes.h"
#include <list>
#include <map>
#include <string>
#include "SDL_mixer.h"
#include "audio_processor.h"
#include "hud.h"
#include "soundSource.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32
#define HISTORY_BUFFER_SIZE 43 		//size of sound energy buffer
#define SENSITIVITY 1.1 			// sensitivity for beat detection
#define CHUNK 2*4096 					//size of audio stream chunks
#define BANDS 6
#define INSERT_YOUR_SONG_PATH_HERE "test.mp3"

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
	audio_processor *au;
	SoundSourceIterator * soundIter;
private:
	SDL_Surface* pScreen;
	
	background *bg;
	std::map<std::string, void*> resources;
	hud *h;
};

SDL_Surface* LoadImage(char* filename);
void DrawToSurface( Uint32 x, Uint32 y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip = NULL);


#endif /* game_H_ */
