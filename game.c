#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image player_closed = NULL;
CP_Image player_opened = NULL;
CP_Image dumpling_closed = NULL;
CP_Image dumpling_opened = NULL;
CP_Image boss_closed = NULL;
CP_Image boss_opened = NULL;
CP_Image fireball = NULL;
extern CP_Image background;
int fireball_x = 300;

// attack , defense , current_hp , max_hp
struct Characters player = { 100.0, 50.0, 1000.0, 1000.0 };
// enemy values to be updated based on user testing
struct Characters dumpling = { 80.0, 50.0, 1000.0, 1000.0 };
struct Characters butter = { 80.0, 50.0, 1500.0, 1500.0 };
struct Characters boss = { 120.0, 60.0, 1800.0, 1800.0 };

void Game_Init(void)
{
    player_closed = CP_Image_Load("./Assets/player-closed.png");
    player_opened = CP_Image_Load("./Assets/player-open.png");
    dumpling_closed = CP_Image_Load("./Assets/dumpling-closed.png");
    dumpling_opened = CP_Image_Load("./Assets/dumpling-open.png");
    boss_closed = CP_Image_Load("./Assets/boss-closed.png");
    boss_opened = CP_Image_Load("./Assets/boss-open.png");
    fireball = CP_Image_Load("./Assets/fireball.png");
}

void Game_Update(void)
{
    //CP_Settings_Tint(CP_Color_Create(0, 0, 0, 200));
    CP_Color darkRed = CP_Color_Create(255, 0, 0, 255);
    CP_Color darkGreen = CP_Color_Create(0, 255, 0, 255);
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    //setting background
    CP_Image_Draw(background, 800, 450, 1600, 900, 255);

    //setting text for rounds
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Round: 1/10", 800, 130);

    //setting text for player defence
    CP_Settings_TextSize(30);
    char defenceText[50];
    snprintf(defenceText, sizeof(defenceText), "Defence: %.2f", CalculateCurrentDefence(player.defense, 0));
    CP_Font_DrawText(defenceText,
        PLAYER_X_AXIS - 20,
        PLAYER_Y_AXIS - PLAYER_HEIGHT - 15);

    //setting text for player attack
    char attackText[50];
    snprintf(attackText, sizeof(attackText), "Attack: %.2f", CalculateCurrentAttack(player.attack, 0));
    CP_Font_DrawText(attackText,
        PLAYER_X_AXIS - 20,
        PLAYER_Y_AXIS - PLAYER_HEIGHT + 15);

    //current level status, bar at the top
    CP_Settings_RectMode(CP_POSITION_CENTER);
    DrawRectangle(CP_Color_Create(100, 100, 100, 100), 800, 100, 1000, 10, 10);
    CP_Image_Draw(player_opened, PLAYER_X_AXIS, 100, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);
    CP_Image_Draw(dumpling_opened, 600, 90, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);

    //shadow for player
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(PLAYER_X_AXIS + 25, PLAYER_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    //player's healthbar
    HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, CalculateCurrentHP(player.current_hp, 0), CalculateCurrentMaxHP(player.max_hp, 0));

    //collision check for player fireball and enemy
    bool fireballHitEnemy = AreCirclesIntersecting(fireball_x, PLAYER_Y_AXIS, 90 / 2, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH / 2);

    //if not colliding, fireball continues to travel forward. else, enemy's hp is reduced based off player's attack
    if (!fireballHitEnemy) {
        fireball_x += 10;
        CP_Image_Draw(fireball, fireball_x, PLAYER_Y_AXIS, 90, 90, 255);
        //player open mouth when shooting fireball
        CP_Image_Draw(player_opened, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
        HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, 0), CalculateCurrentMaxHP(dumpling.max_hp, 0));
    } else {
        //player close mouth when fireball is shot
        CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
        //base health should be current enemy health, and base attack should be current player attack
        HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, player.attack), CalculateCurrentMaxHP(dumpling.max_hp, 0));
    }

    //shadow for enemy
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(ENEMY_X_AXIS + 25, ENEMY_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);
    
    //enemy closed mouth
    CP_Image_Draw(dumpling_closed, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);

    //CP_Engine_SetNextGameState(Upgrades_Init, Upgrades_Update, Upgrades_Exit);
}

void Game_Exit(void)
{
    CP_Image_Free(player_opened);
    CP_Image_Free(player_closed);
    CP_Image_Free(dumpling_closed);
    CP_Image_Free(dumpling_opened);
}