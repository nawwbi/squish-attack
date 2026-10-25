#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include "mainmenu.h"
#include "game.h"
#include <stdbool.h>


CP_Image defence_button;
CP_Image attack_button;
CP_Image hp_button;
CP_Font font;
int numSkills;

struct Upgrades skill[] = {
	{"+5% Defence", 350, 220 },
	{ "+10% Attack", 800, 680 },
	{ "+20% Max HP", 1250, 1130 }
};

void Upgrades_Init(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	defence_button = CP_Image_Load("./Assets/defence_button.png");
	attack_button = CP_Image_Load("./Assets/attack_button.png");
	hp_button = CP_Image_Load("./Assets/hp_button.png");
	font = CP_Font_Load("Assets/Exo2-Regular.ttf");

	skill[0].image = defence_button;
	skill[1].image = attack_button;
	skill[2].image = hp_button;

	numSkills = sizeof(skill) / sizeof(skill[0]);
}

void Upgrades_Update(void) {
	CP_Color black = CP_Color_Create(0, 0, 0, 255);
	for (int i = 0; i < numSkills; i++) {
		SkillUpgradeButton(skill[i].image, skill[i].image_x, BUTTON_Y_AXIS, BUTTON_WIDTH, BUTTON_HEIGHT, black, font, skill[i].name, skill[i].text_x, 520);
	}
	
	//SkillUpgradeButton(attack_button, 800, BUTTON_Y_AXIS, BUTTON_WIDTH, BUTTON_HEIGHT, black, font, "+10% Attack", 680, 520);
	//SkillUpgradeButton(hp_button, 1250, BUTTON_Y_AXIS, BUTTON_WIDTH, BUTTON_HEIGHT, black, font, "+20% Max HP", 1130, 520);

}

void Upgrades_Exit(void) {
	CP_Image_Free(defence_button);
}