/*
 *  testSoundSource.cpp
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 25/04/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "soundSource.h"


int main(int argc, char *argv[])
{
	SoundSource * s = new SoundSource("test.mp3");
	SoundSourceIterator * iter;
	
	iter = s->iter(1024);
	while(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			std::cout << sample->sample[i];
		}
	}

}

