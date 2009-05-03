/*
 *  ResourceBundle.h
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 03/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <map>
/*
filename:resources/background/Nebula1.bmp
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


class ResourceBundle
{
public:
	ResourceBundle ();
	virtual ~ResourceBundle ();
	
	std::map<char*, char*> data;
};