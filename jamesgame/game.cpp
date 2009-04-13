#include "game.h"

void * game::getResource(std::string s){
	if (resources.find(s)==resources.end())
	{
		resources[s]=LoadImage((char*)s.c_str());
	}
	return resources[s];
}


void game::gameloop(){
	Mix_PlayMusic( music, -1 );

	while(shouldIQuit() == false)
	{
		if (1==(rand()%40)){
			for (int i =0; i<rand()%10; i++)
			{
				baddie *b = new baddie(this);
				sprite_list.push_back(b);
			}
		}

		keys = SDL_GetKeyState( NULL );

		bg->updateBackground(getWindowSurface());

		std::list<sprite*>::iterator i;

		for(i=sprite_list.begin(); i != sprite_list.end(); ++i) {
			sprite *bees = *i;
			if (bees->removeMe)
			{
				i = sprite_list.erase(i);
				delete bees;
			}
		}

		for(i=sprite_list.begin(); i != sprite_list.end(); ++i) {
			(*i)->moveSprite();
		}

		for(i=sprite_list.begin(); i != sprite_list.end(); ++i) {
			(*i)->renderSprite();
		}
		Flip();
	}
}

int main(int argc, char* argv[])
{
	game at;
	at.gameloop();
	return 0;
}

bool game::shouldIQuit()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				return true;
			}
		}
	}
	return false;
}

SDL_Surface* LoadImage(char* filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimisedImage = NULL;
	loadedImage = SDL_LoadBMP(filename);

	if( loadedImage != NULL )
	{
		optimisedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}

	return optimisedImage;
}

void DrawToSurface( Uint32 x, Uint32 y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( pSource, clip, pDest, &offset );
}

SDL_Surface* game::getWindowSurface()
{
	return pScreen;
}

game::game()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption( "Bass Invaders", NULL );
	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);
	bg = new background(SCREEN_WIDTH, SCREEN_HEIGHT);
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, CHUNK/4 );
    music = Mix_LoadMUS(INSERT_YOUR_SONG_PATH_HERE);
	sprite *s = new sprite(this);
	sprite_list.push_back(s);
}

game::~game()
{
	delete bg;
	SDL_Quit();
}

void game::Flip()
{
	SDL_Flip(pScreen);
}
