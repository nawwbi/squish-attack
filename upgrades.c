#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include "mainmenu.h"
#include "game.h"
#include "levelintro.h"
#include "debug.h"
#include <stdbool.h>

int numSkills = 0;
int clicked = 0;
int minimum = 0;
int maximum;
float totalRarity = 0;
enum { ARRAY_SIZE = 3 };
int random_list[ARRAY_SIZE] = {0, 0, 0};

CP_Image
defence_icon,
hp_icon,
attack_icon,
heal_icon;

//defence upgrade image x axis, text x axis, text y axis
#define DEFENCE_IMAGE_X   360
#define DEFENCE_TEXT_X    230
#define DEFENCE_TEXT_Y    360

struct Upgrades upgrades[] = {
    { DEFENCE, "+5% Defence", 0.05f, COMMON },
    { DEFENCE, "+10% Defence", 0.1f, RARE },
    { DEFENCE, "+15% Defence", 0.15f, VERY_RARE },

    { ATTACK, "+5% Attack", 0.05f, COMMON },
    { ATTACK, "+10% Attack", 0.1f, RARE },
    { ATTACK, "+15% Attack", 0.15f, VERY_RARE },

    { HP, "+10% Max HP", 0.1f, COMMON },
    { HP, "+15% Max HP", 0.15f, RARE },
    { HP, "+20% Max HP", 0.2f, VERY_RARE },

    { HEAL, "+10% HP Heal", 0.1f, COMMON },
    { HEAL, "+20% HP Heal", 0.2f, RARE },
    { HEAL, "+30% HP Heal", 0.3f, VERY_RARE },

    { BASIC_ATTACK_ITERATION, "+1 Attack", 1.0f, COMMON },
    { BASIC_ATTACK_ITERATION, "+2 Attack", 2.0f, RARE },
    { BASIC_ATTACK_ITERATION, "+3 Attack", 3.0f, VERY_RARE },
};

struct Upgrades displaying_upgrades[ARRAY_SIZE];

// helper funtion to only be used in this file
// returns the image of the upgrade based on the upgrade_enum
static CP_Image Upgrade_Icon(int upgrade_enum) {
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

    case BASIC_ATTACK_ITERATION:
        image = attack_icon;
        break;

	default:
		image = defence_icon;
    }

    return image;
}

static void Pick_Random_Upgrades(void) {
    numSkills = sizeof(upgrades) / sizeof(upgrades[0]);

	//getting total weight of all upgrades
    float total_weight = 0.0f;
    for (int i = 0; i < numSkills; ++i) {
        total_weight += upgrades[i].rarity;
    }

    
    for (int slot = 0; slot < ARRAY_SIZE; ++slot) {
		//picks random float between 0 and total weight
        float roll = CP_Random_RangeFloat(0.0f, total_weight);
        float cumulative = 0.0f;

        //going through all of the skills
        for (int i = 0; i < numSkills; ++i) {
            cumulative += upgrades[i].rarity;
            //adding weight of rarity until it is greater or equal to random roll
            if (roll <= cumulative) {
				//add the skill to the random list for the current slot
                random_list[slot] = i;
				//break loop once a skill is selected for the current slot
                break;
            }
        }
    }
}

void Upgrades_Init(void) { 
    defence_icon = CP_Image_Load("./Assets/defence-icon.png");
    hp_icon = CP_Image_Load("./Assets/hp-icon.png");
    attack_icon = CP_Image_Load("./Assets/fireball.png");
    heal_icon = CP_Image_Load("./Assets/heal-icon.png");

	Pick_Random_Upgrades();

    float current_image_x = DEFENCE_IMAGE_X;
    float current_text_x = DEFENCE_TEXT_X;
    float current_text_y = DEFENCE_TEXT_Y;

	for (int i = 0; i < ARRAY_SIZE; ++i) {
        displaying_upgrades[i].upgrade_enum = upgrades[random_list[i]].upgrade_enum;
        displaying_upgrades[i].details = upgrades[random_list[i]].details;
        displaying_upgrades[i].value = upgrades[random_list[i]].value;
        displaying_upgrades[i].rarity = upgrades[random_list[i]].rarity;
        displaying_upgrades[i].image_x = current_image_x;
        displaying_upgrades[i].text_x = current_text_x;
        displaying_upgrades[i].icon_x = current_text_y;
        displaying_upgrades[i].icon = Upgrade_Icon(upgrades[random_list[i]].upgrade_enum);

        current_image_x += 450;
        current_text_x += 450;
        current_text_y += 450;
	}
}

void Upgrades_Update(void) { 
    static int upgradeEnum = 0;
	static float selectedValue = 0;

    CP_Color black = CP_Color_Create(0, 0, 0, 255);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
        CP_Settings_RectMode(CP_POSITION_CENTER);

        clicked = Is_Area_Clicked(
            displaying_upgrades[i].image_x,
            BUTTON_Y_AXIS,
            300,
            500,
            CP_Input_GetMouseX(),
            CP_Input_GetMouseY()
        );

        if (clicked) {
            upgradeEnum = displaying_upgrades[i].upgrade_enum;
			selectedValue = displaying_upgrades[i].value;
            break;
        }

        Skill_Upgrade_Button(
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
            ICON_HEIGHT,
            displaying_upgrades[i].value,
            CP_Color_Create(0, 0, 0, 255),
            displaying_upgrades[i].rarity
        );
    }

    if (clicked && upgradeEnum >= 0) {

        switch (upgradeEnum) {

        case DEFENCE:
            player.defence = Calculate_Current_Defence(player.defence, selectedValue);
            break;

        case ATTACK:
            player.attack = Calculate_Current_Attack(player.attack, selectedValue);
            break;

        case HP:
            player.max_hp = Calculate_Current_Max_HP(player.max_hp, selectedValue);
            break;

        case HEAL:
            player.current_hp = Calculate_Healing(player.current_hp, player.max_hp, selectedValue);
            break;

        case BASIC_ATTACK_ITERATION:
			player.skills[BASIC_ATTACK].iteration += (int)selectedValue;
            break;
        }

        upgradeEnum = 0;
        selectedValue = 0;
        clicked = 0;
        
        CP_Engine_SetNextGameState(Level_Intro_Init, Level_Intro_Update, Level_Intro_Exit);

        return;
    }

    Debug_Update();
    Debug_Draw();
}

void Upgrades_Exit(void) { 
    CP_Image_Free(defence_icon);
    CP_Image_Free(hp_icon);
    CP_Image_Free(attack_icon);
}