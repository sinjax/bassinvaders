#include "game.h"

void * game::getResource(std::string s){
	if (resources.find(s)==resources.end())
	{
		resources[s]=LoadImage((char*)s.c_str());
	}
	return resources[s];
}
void band_separate( void *udata, uint8_t *stream, int len){
	((audio_processor*)udata)->process(stream);
}
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

void game::musicDebug()
{
	std::stringstream debugStream;
	debugStream << "Packets Read: " << this->soundIter->packetsRead << "/"
				<< this->soundIter->source->packetQueue.nb_packets 
				<< "(" << 100.0 * (double)(this->soundIter->packetsRead) / (double)(this->soundIter->source->packetQueue.nb_packets) << "%)" << std::endl;
	debugStream << "Bytes Read: " << this->soundIter->read << "/" 
				<< this->soundIter->source->packetQueue.size 
				<< "(" << 100.0 * (double)(this->soundIter->read) / (double)(this->soundIter->source->packetQueue.size)<<  "%)" << std::endl;
	h->displayText(
		MDEBUG_X,
		MDEBUG_Y,
		debugStream.str()
	);
}

void game::gameloop(){
   // Mix_SetPostMix(band_separate, &au);

	//Mix_PlayMusic( music, -1 );
	int score = 0;
	int cooldown = 20;

	while(shouldIQuit() == false)
	{
		if ((cooldown<0)&&this->au->poll_beat(0)){
			//srand(au.poll_sig(4));
			for (int i =0; i<rand()%10; i++)
			{
				baddie *b = new baddie(this);
				sprite_list.push_back(b);
				cooldown = 10;
			}
		}
		cooldown--;
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
		std::stringstream scoreStream;
		scoreStream << "Score: " << std::endl << score/10;
		
		h->displayText(10,10,scoreStream.str());
		musicDebug();
		if ((*sprite_list.begin())->currentState==IDLE){
			if(2*(*sprite_list.begin())->xpos>SCREEN_WIDTH)
			{ score+=2;}
			else {score++;}}
		if ((*sprite_list.begin())->currentState==DYING)score-=3;
		if ((*sprite_list.begin())->currentState==DEAD)score-=6;
		if (score <0) score = 0;

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

void MusicPlayer(void *udata, Uint8 *stream, int len)
{
	SoundSourceIterator* iter = ((game*)udata)->soundIter;
	if(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			stream[i] = sample->sample[i];
		}
	}

	band_separate(((game*)udata)->au, stream, len);
}

game::game()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption( "Bass Invaders", NULL );
	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);
	bg = new background(SCREEN_WIDTH, SCREEN_HEIGHT);
	SoundSource * source = new SoundSource(INSERT_YOUR_SONG_PATH_HERE);
    Mix_OpenAudio( source->spec.freq, 
				   MIX_DEFAULT_FORMAT, 
				   source->spec.channels, 
				   source->spec.samples);
	int historyBuffer = 1.0 / ((double)(source->spec.samples)/(double)(source->spec.freq));
	//double sensit = SENSITIVITY;
	double sensit = 1.6;
	au = new audio_processor (source->spec.samples*4, BANDS, historyBuffer, sensit );
    //music = Mix_LoadMUS(INSERT_YOUR_SONG_PATH_HERE);
    
    //soundIter = new SoundSourceIterator(source, source->spec.samples*4);
	/*soundIter = source->iter(source->spec.samples*4);
	AVPacket pkt;
	for(int waste = 0; waste < 600; waste++)
	{
		soundIter->nextPacket(&pkt,false);
		soundIter->read += pkt.size;
	}*/
	soundIter = source->iterBytes((source->spec.freq*source->spec.channels)*0, // start
								  (source->spec.freq*source->spec.channels)*6, // stop
								  source->spec.samples*4);
    Mix_HookMusic(MusicPlayer, this);

	sprite *s = new sprite(this);
	sprite_list.push_back(s);
	SDL_Color c = {55, 255, 25};
	h = new hud("Batang.ttf", 20, c, pScreen);
}

game::~game()
{
	delete bg; delete h;
	SDL_Quit();
}

void game::Flip()
{
	SDL_Flip(pScreen);
}
