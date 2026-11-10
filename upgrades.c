#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include "mainmenu.h"
#include "game.h"
#include <stdbool.h>

int numSkills = 0;
int clicked = 0;
int minimum = 0;
int maximum;
int random_list[3] = {0, 0, 0};

CP_Image
defence_icon,
hp_icon,
attack_icon,
heal_icon;

//defence upgrade image x axis, text x axis, text y axis
#define DEFENCE_IMAGE_X   360
#define DEFENCE_TEXT_X    220
#define DEFENCE_TEXT_Y    360

struct Upgrades upgrades[] = {
    { DEFENCE, "+5% Defence" },
    { ATTACK, "+10% Attack" },
    { HP, "+20% Max HP" },
    { HEAL, "+30% HP Heal" },
};

struct Upgrades displaying_upgrades[3];

CP_Image upgrade_icon(int upgrade_enum) {
    CP_Image image;
    switch (upgrade_enum) {

    case DEFENCE:
        image = defence_icon;
        break;

    case ATTACK:
        image = attack_icon;
        break;

    case HP:
        image = hp_icon;
        break;

    case HEAL:
        image = heal_icon;
        break;

	default:
		image = defence_icon;
    }

    return image;
}

void Upgrades_Init(void) { 
    defence_icon = CP_Image_Load("./Assets/defence-icon.png");
    hp_icon = CP_Image_Load("./Assets/hp-icon.png");
    attack_icon = CP_Image_Load("./Assets/fireball.png");
    heal_icon = CP_Image_Load("./Assets/heal-icon.png");

    numSkills = sizeof(upgrades) / sizeof(upgrades[0]);
	maximum = numSkills - 1;

	for (int i = 0; i < numSkills; ++i) {
		int random = minimum + (rand() % (maximum - minimum + 1));
		random_list[i] = random;
	}

    int current_image_x = DEFENCE_IMAGE_X;
    int current_text_x = DEFENCE_TEXT_X;
    int current_text_y = DEFENCE_TEXT_Y;

	for (int i = 0; i < 3; ++i) {
        displaying_upgrades[i].upgrade_enum = upgrades[random_list[i]].upgrade_enum;
        displaying_upgrades[i].details = upgrades[random_list[i]].details;
        displaying_upgrades[i].image_x = current_image_x;
        displaying_upgrades[i].text_x = current_text_x;
        displaying_upgrades[i].icon_x = current_text_y;
        displaying_upgrades[i].icon = upgrade_icon(upgrades[random_list[i]].upgrade_enum);

        current_image_x += 450;
        current_text_x += 450;
        current_text_y += 450;
	}
}

void Upgrades_Update(void) { 
    static int upgradeEnum = 0;

    CP_Color black = CP_Color_Create(0, 0, 0, 255);

    for (int i = 0; i < 3; ++i) {
        CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
        CP_Settings_RectMode(CP_POSITION_CENTER);

        clicked = IsAreaClicked(
            displaying_upgrades[i].image_x,
            BUTTON_Y_AXIS,
            300,
            500,
            CP_Input_GetMouseX(),
            CP_Input_GetMouseY()
        );

        if (clicked) {
            upgradeEnum = upgrades[i].upgrade_enum;
            break;
        }

        SkillUpgradeButton(
            displaying_upgrades[i].upgrade_enum,
            displaying_upgrades[i].image_x,
            BUTTON_Y_AXIS,
            BUTTON_WIDTH,
            BUTTON_HEIGHT,
            black,
            displaying_upgrades[i].details,
            displaying_upgrades[i].text_x,
            520,
            displaying_upgrades[i].icon,
            displaying_upgrades[i].icon_x,
            ICON_Y_AXIS,
            ICON_WIDTH,
            ICON_HEIGHT
        );
    }

    if (clicked && upgradeEnum >= 0) {

        switch (upgradeEnum) {

        case DEFENCE:
            player.defence = CalculateCurrentDefence(player.defence, 0.05);
            break;

        case ATTACK:
            player.attack = CalculateCurrentAttack(player.attack, 0.1);
            break;

        case HP:
            player.max_hp = CalculateCurrentMaxHP(player.max_hp, 0.2);
            break;

        case HEAL:
            player.current_hp = CalculateHealing(player.current_hp, player.max_hp, 0.3);
            break;
        }

        upgradeEnum = 0;
        clicked = 0;
        
        CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);

        return;
    }
}

void Upgrades_Exit(void) { 
    CP_Image_Free(defence_icon);
    CP_Image_Free(hp_icon);
    CP_Image_Free(attack_icon);
}