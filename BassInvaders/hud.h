/*
 * hud.h
 *
 *  Created on: Apr 15, 2009
 *      Author: Darren Golbourn
 *
 *	Description: This class defines a user display.  Use displayText to display printf formatted strings to the
 *	game window.  If there are other SDL sprites or things which should be displayed (i.e. icons and such) then
 *	register them with the HUD object using registerSurface.  When the draw() method is called everything registered
 *	with the HUD will be displayed at once.
 */

#ifndef HUD_H_
#define HUD_H_

#include <SDL/SDL.h>
#include "SDL_ttf.h"
#include <list>
#include <string>
#include "game.h"

/*
 * Stores all the data needed to display an SDL_Surface on the screen
 */
typedef struct component{
	SDL_Rect offset;	// x,y offset
	SDL_Rect clip;		// crop region for sprite sheets etc.
	SDL_Surface* component;// address of SDL_Surface to display
}component_t;

class hud {
	SDL_Surface *message;// temporary text buffer
	SDL_Surface *baseSurface;// the Screen on to which the HUD is to be drawn

	TTF_Font *font;// font for displayText
	SDL_Color textColor;//color of displayText

	std::list<component_t> components;//list of surfaces to display on call to draw()
public:
	hud(const char* fnt, int sz, SDL_Color c, SDL_Surface* dest);
	virtual ~hud();
	void displayText(int x, int y, char * text,...);	
	void draw();
	void registerSurface(Uint32 x, Uint32 y, SDL_Surface* component, SDL_Rect* clip);
};

#endif /* HUD_H_ */
