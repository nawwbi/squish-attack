#pragma once
#include <stdio.h>
#include <string.h>

enum UpgradeEnum {
	DEFENCE,
	ATTACK,
	HP,
	HEAL,
	BASIC_ATTACK_ITERATION
};

// int upgrade_enum, char* details, 
// int image_x, int text_x, int icon_x
struct Upgrades {
	int      upgrade_enum;
	char*    details;
	float    value;
	int      rarity;
	float    image_x;
	float    text_x;
	float    icon_x;
	CP_Image icon;
};

void Upgrades_Init(void);

void Upgrades_Update(void);

void Upgrades_Exit(void);