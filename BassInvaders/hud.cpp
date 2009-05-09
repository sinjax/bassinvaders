/*
 * hud.cpp
 *
 *  Created on: Apr 15, 2009
 *      Author: Darren Golbourn
 */

#include "hud.h"
/* for now, when initializing the HUD, choose a single point and colour of font */
hud::hud(const char* fnt, int sz, SDL_Color c, SDL_Surface* dest) {
	TTF_Init();
	font = TTF_OpenFont( fnt, sz );
    textColor = c;
    baseSurface = dest;
}

hud::~hud() {
    TTF_CloseFont( font );
    TTF_Quit();
}

/*
 * display text formatted in printf format at location (x,y) (i.e. to print changing numerical data like scores)
 */
void hud::displayText(int x, int y, char* text,...)
{
	char *buffer;
	va_list args;
	va_start (args, text);

/*
 * due to cross-platform compatibility, some systems do not have access to
 * certain extensions to glibc.  In this case use vsprintf with a fixed sized buffer for now.
 */
#if !defined(_ANSI_SOURCE) && (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
	vasprintf(&buffer,text, args);
#else
	buffer = malloc(1024*sizeof(char));
	vsprintf(buffer, text, args);
#endif
	char * pch = strtok (buffer,"\n");
	int currentY = y;
	int fontHeight = TTF_FontHeight(font);
	while (pch != NULL)
	{
		message = TTF_RenderText_Solid( font, pch, textColor );
		SDL_Rect offset;
		offset.x = x;
		offset.y = currentY;
		SDL_BlitSurface( message, NULL, baseSurface, &offset );
		SDL_FreeSurface( message );
		pch = strtok (NULL, "\n");
		currentY += fontHeight;
	}
	free(buffer);
	va_end (args);
}

/**
 * Use this, along with to register SDL surfaces to be displayed on the HUD when draw is called
 */
void hud::registerSurface(Uint32 x, Uint32 y, SDL_Surface* component, SDL_Rect* clip = NULL){
	SDL_Rect temp ={x,y,0,0};
	component_t c;
	c.offset = temp;
	c.clip = *clip;
	c.component = component;
	components.push_back(c);
}

void hud::draw(){
	std::list<component_t>::iterator i;

	for(i = components.begin(); i != components.end(); ++i) {
		component_t bees = *i;
		DrawToSurface(bees.offset.x, bees.offset.y, bees.component, baseSurface, &(bees.clip));
	}
}
