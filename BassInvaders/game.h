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
//#include "audio_processor.h"
#include "hud.h"
#include "soundSource.h"
#include "BeatDetector.h"
#include "BandPassFilterFFT.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32
#define INSERT_YOUR_SONG_PATH_HERE "test.mp3"
#define SENSITIVITY 1.3
#define MDEBUG_X 0
#define MDEBUG_Y SCREEN_HEIGHT - 60

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
	BandPassFilterFFT *fft;
	BeatDetector *beat;
	SoundSourceIterator * soundIter;
private:
	SDL_Surface* pScreen;
	void musicDebug();

	background *bg;
	std::map<std::string, void*> resources;
	hud *h;
};

SDL_Surface* LoadImage(char* filename);
void DrawToSurface( Uint32 x, Uint32 y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip = NULL);


#endif /* game_H_ */
