/*
 *  ResourceBundle.cpp
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 03/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceBundle.h"

ResourceBundle::ResourceBundle(char * infoFile)
{
	FILE* fp;
	if((fp = fopen(infoFile, "r")) == NULL)
	{
		printf("Couldn't open file\n");
		return;
	}
	char buffer[255] = {0};
	char key[255] = {0};
	char value[255] = {0};
	while(!feof(fp))
	{
		fgets(buffer, 255, fp);
		char * key = strtok(buffer,":");
		char * value = strtok(NULL,":");

		/*
		filename:some/file/name
		colorkey:(1,0,255,0)
		drawbox:(0,0,128,128)
		repeatpoint:(768)
		tiling:(0,0)
		drawpos:(640,240)
		scrollRatio:(1.1)
		numberofstates:1
		state:1
		nextstate:1
		numberofanimationsteps:3
		ticksperstep:350
		sheetstartsat:(1,1)
		spritesize:(47,47)
		numberofrects:1
		rect:(1,1,47,47)*/
		if(strcmp(key,"filename") == 0)
		{
			char filename[255];
			sscanf(value, "%s", filename);
		}
		else if(strcmp(key,"colorkey") == 0)
		{
			// load a 4 element array
		}
		else if(strcmp(key,"drawbox") == 0)
		{
			// load a 4 element array
		}

		/*
		if(commands[key] == IMAGE)
		if(commands[key] == ARRAY)
		if(commands[key] == MUSIC)
		*/
	}

}

ResourceBundle::~ResourceBundle()
{
}





