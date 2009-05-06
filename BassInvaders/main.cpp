/*
 * main.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "BassInvaders.h"
int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);
	ResourceBundle * b = new ResourceBundle("resources/hero/heroclass.info");
	printf("Test heroclass\n");
	for(std::map<std::string, void*>::const_iterator it = b->data.begin(); it != b->data.end(); ++it)
	{
		
		if(b->supportedTypes[it->first] == STRING)
		{
			std::cout << it->first;
			std::cout << " : " << (char*)(it->second) << std::endl;
		} 
		else if(b->supportedTypes[it->first] == RESOURCE)
		{
			std::cout << "RESOURCE: " << std::endl;
			ResourceBundle * r = (ResourceBundle*)(b->data[it->first]);
			for(std::map<std::string, void*>::const_iterator rit = r->data.begin(); rit != r->data.end(); ++rit)
			{
				if(b->supportedTypes[rit->first] == STRING)
				{
					std::cout << "\t" << rit->first;
					std::cout << " : " << (char*)(rit->second) << std::endl;
				}
			}
		}
		else{
			std::cout << "OTHER!!" << std::endl;
		}
        
    }
	return 0;
	BassInvaders bi;
	bi.goGameGo();
	return EXIT_SUCCESS;
}
