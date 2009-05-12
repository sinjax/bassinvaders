/*
 * hud.cpp
 *
 *  Created on: Apr 15, 2009
 *      Author: Darren Golbourn
 */

#include "hud.h"
/* for now, when initializing the HUD, choose a single point size and colour of font */
hud::hud(const char* fnt, int sz, SDL_Color c, SDL_Surface* dest) {
	TTF_Init();
	font = TTF_OpenFont( fnt, sz );

	if(!font)
	{
		DebugPrint(("couldn't open font %s\n", fnt));
	}
	else
	{
		DebugPrint(("opened font %s\n", fnt));
	}

    textColor = c;
    baseSurface = dest;
}

hud::~hud() {
    TTF_CloseFont( font );
    TTF_Quit();
}

static int int_vasprintf (char ** result, const char * format, va_list args);

/*
 * display text formatted in printf format at location (x,y) (i.e. to print changing numerical data like scores)
 */
void hud::displayText(int x, int y, char* text,...)
{
	char *buffer = NULL;
	va_list args;
	va_start (args, text);

	int_vasprintf(&buffer,text, args);
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
 * Use this to register SDL surfaces to be displayed on the HUD when draw is called
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

static int int_vasprintf (char ** result, const char * format, va_list args){
	const char *p = format;
	/* Add one to make sure that it is never zero, which might cause malloc
	 to return NULL.  */
	int total_width = strlen (format) + 1;
	va_list ap;

	memcpy (&ap, &args, sizeof (va_list));

	while (*p != '\0')
    {
		if (*p++ == '%')
		{
			while (strchr ("-+ #0", *p))
			++p;
			if (*p == '*')
			{
				++p;
				total_width += abs (va_arg (ap, int));
			}
			else
				total_width += strtoul (p, (char **) &p, 10);
			if (*p == '.')
			{
				++p;
				if (*p == '*')
				{
					++p;
					total_width += abs (va_arg (ap, int));
				}
				else
					total_width += strtoul (p, (char **) &p, 10);
				}
			while (strchr ("hlL", *p))
			++p;
			/* Should be big enough for any format specifier except %s and floats.  */
			total_width += 30;
			switch (*p)
			{
				case 'd':
				case 'i':
				case 'o':
				case 'u':
				case 'x':
				case 'X':
				case 'c':
					(void) va_arg (ap, int);
				break;
				case 'f':
				case 'e':
				case 'E':
				case 'g':
				case 'G':
					(void) va_arg (ap, double);
			  /* Since an ieee double can have an exponent of 307, we'll
			 make the buffer wide enough to cover the gross case. */
					total_width += 307;
				break;
				case 's':
					total_width += strlen (va_arg (ap, char *));
				break;
				case 'p':
				case 'n':
					(void) va_arg (ap, char *);
				break;
			}
			p++;
		}
    }

	*result = (char*)malloc (total_width);
	if (*result != NULL)
		return vsprintf (*result, format, args);
	else
		return 0;
}

