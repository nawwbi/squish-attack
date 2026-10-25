#pragma once
#include <stdio.h>
#include <string.h>

struct Upgrades {
	char* name;
	int image_x;
	int text_x;
	CP_Image image;

};

void Upgrades_Init(void);

void Upgrades_Update(void);

void Upgrades_Exit(void);