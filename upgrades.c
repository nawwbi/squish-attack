#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include "mainmenu.h"
#include "game.h"
#include <stdbool.h>


CP_Image button;

CP_Image defence_icon;
CP_Image hp_icon;
CP_Image attack_icon;

int numSkills;

struct Upgrades upgrade[] = {
	{ DEFENCE, "+5% Defence", 360, 220, 360 },
	{ ATTACK, "+10% Attack", 810, 670, 810 },
	{ HP, "+20% Max HP", 1260, 1120, 1260 }
};

void Upgrades_Init(void) {
	
	button = CP_Image_Load("./Assets/defence_button.png");
	defence_icon = CP_Image_Load("./Assets/defence-icon.png");
	hp_icon = CP_Image_Load("./Assets/hp-icon.png");
	attack_icon = CP_Image_Load("./Assets/fireball.png");

	upgrade[0].image = button;
	upgrade[1].image = button;
	upgrade[2].image = button;

	upgrade[0].icon = defence_icon;
	upgrade[1].icon = attack_icon;
	upgrade[2].icon = hp_icon;

	numSkills = sizeof(upgrade) / sizeof(upgrade[0]);
}

void Upgrades_Update(void) {
	CP_Settings_NoTint();
	CP_Color black = CP_Color_Create(0, 0, 0, 255);
	for (int i = 0; i < numSkills; i++) {
		SkillUpgradeButton(
			upgrade[i].skill,

			upgrade[i].image,
			upgrade[i].image_x,
			BUTTON_Y_AXIS, 
			BUTTON_WIDTH, 
			BUTTON_HEIGHT, 

			black,
			upgrade[i].name,
			upgrade[i].text_x,
			520,

			upgrade[i].icon,
			upgrade[i].icon_x,
			ICON_Y_AXIS,
			ICON_WIDTH,
			ICON_HEIGHT
		);
	}
}

void Upgrades_Exit(void) { 
	CP_Image_Free(button);
	CP_Image_Free(defence_icon);
	CP_Image_Free(hp_icon);
	CP_Image_Free(attack_icon);
}