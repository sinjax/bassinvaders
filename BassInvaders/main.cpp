/*
 * main.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "BassInvaders.h"
int main(int argc, char* argv[])
{
	ResourceBundle * b = new ResourceBundle("resources/hero/heroclass.info");
	printf("Test heroclass\n");
	printf("Health: %s\n",(char*)(b->data["health"]));
	printf("Damage: %s\n",(char*)(b->data["attackdamage"]));
	return 0;
	BassInvaders bi;
	bi.goGameGo();
	return EXIT_SUCCESS;
}
