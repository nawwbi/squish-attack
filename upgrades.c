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

CP_Image
button,
defence_icon,
hp_icon,
attack_icon;

//defence upgrade image x axis, text x axis, text y axis
#define DEFENCE_IMAGE_X   360
#define DEFENCE_TEXT_X    220
#define DEFENCE_TEXT_Y    360

//attack upgrade image x axis, text x axis, text y axis
#define ATTACK_IMAGE_X   810
#define ATTACK_TEXT_X    670
#define ATTACK_TEXT_Y    810

//hp upgrade image x axis, text x axis, text y axis
#define HP_IMAGE_X   1260
#define HP_TEXT_X    1120
#define HP_TEXT_Y    1260

Upgrades upgrades[] = {
    { DEFENCE, "+5% Defence", DEFENCE_IMAGE_X, DEFENCE_TEXT_X, DEFENCE_TEXT_Y },
    { ATTACK, "+10% Attack", ATTACK_IMAGE_X, ATTACK_TEXT_X, ATTACK_TEXT_Y },
    { HP, "+20% Max HP", HP_IMAGE_X, HP_TEXT_X, HP_TEXT_Y }
};

void Upgrades_Init(void) { 
    button = CP_Image_Load("./Assets/defence_button.png");
    defence_icon = CP_Image_Load("./Assets/defence-icon.png");
    hp_icon = CP_Image_Load("./Assets/hp-icon.png");
    attack_icon = CP_Image_Load("./Assets/fireball.png");

    upgrades[0].image = button;
    upgrades[1].image = button;
    upgrades[2].image = button;

    upgrades[0].icon = defence_icon;
    upgrades[1].icon = attack_icon;
    upgrades[2].icon = hp_icon;

    numSkills = sizeof(upgrades) / sizeof(upgrades[0]);
}

void Upgrades_Update(void) { 
    static int upgradeEnum = 0;

    CP_Color black = CP_Color_Create(0, 0, 0, 255);

    for (int i = 0; i < numSkills; ++i) {
        CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
        CP_Settings_RectMode(CP_POSITION_CENTER);

        clicked = IsAreaClicked(
            upgrades[i].image_x,
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
            upgrades[i].upgrade_enum,
            upgrades[i].image,
            upgrades[i].image_x,
            BUTTON_Y_AXIS,
            BUTTON_WIDTH,
            BUTTON_HEIGHT,
            black,
            upgrades[i].details,
            upgrades[i].text_x,
            520,
            upgrades[i].icon,
            upgrades[i].icon_x,
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
            player.current_hp = CalculateHealing(player.current_hp, player.max_hp, 0.2);
            break;
        }

        upgradeEnum = 0;
        clicked = 0;
        
        CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);

        return;
    }
}

void Upgrades_Exit(void) { 
    CP_Image_Free(button);
    CP_Image_Free(defence_icon);
    CP_Image_Free(hp_icon);
    CP_Image_Free(attack_icon);
}