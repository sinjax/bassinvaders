#include <iostream>
#include "soundSource.h"
void printBinary(int n)
{
	std::cout << "decimal: " << n << std::endl;
	std::cout << "binary : ";
	for (int i=31; i>=0; i--) 
	{
		int bit = ((n >> i) & 1);
		std::cout << bit;
	}
	std::cout << std::endl;
}

int main(int argc, char *argv[]){
	SoundSource * s = new SoundSource(argv[1]);
	SoundSourceIterator * iter;
	iter = s->iterBytes(44100*0*4,44100*2*4,1024);
	while(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			std::cout << sample->sample[i];
		}
	}
	delete iter;
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
