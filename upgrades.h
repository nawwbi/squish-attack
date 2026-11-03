#pragma once
#include <stdio.h>
#include <string.h>

typedef enum UpgradeEnum {
	DEFENCE,
	ATTACK,
	HP,
	HEAL
} UpgradeEnum;

struct Upgrades {
	int upgrade_enum;
	char* details;
	int image_x;
	int text_x;
	int icon_x;
	CP_Image image;
	CP_Image icon;
};

void Upgrades_Init(void);

void Upgrades_Update(void);

void Upgrades_Exit(void);