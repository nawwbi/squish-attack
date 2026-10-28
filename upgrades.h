#pragma once
#include <stdio.h>
#include <string.h>

typedef enum {
	DEFENCE,
	ATTACK,
	HP,
	HEAL
} Skill;

struct Upgrades {
	int skill;
	char* name;
	int image_x;
	int text_x;
	int icon_x;
	CP_Image image;
	CP_Image icon;
};

void Upgrades_Init(void);

void Upgrades_Update(void);

void Upgrades_Exit(void);