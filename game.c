#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "constants.h"
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
    CP_Color darkRed = CP_Color_Create(255, 0, 0, 255);
    CP_Color darkGreen = CP_Color_Create(0, 255, 0, 255);
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    int width = CP_System_GetWindowWidth();
    int height = CP_System_GetWindowHeight();

    CP_Image_Draw(background, 800, 450, 1600, 900, 255);

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Round: 1/10", 800, 130);

    CP_Settings_RectMode(CP_POSITION_CENTER);
    DrawRectangle(CP_Color_Create(100, 100, 100, 100), 800, 100, 1000, 10, 10);
    CP_Image_Draw(player_opened, PLAYER_X_AXIS, 100, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);
    CP_Image_Draw(dumpling_opened, 600, 90, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(PLAYER_X_AXIS + 25, PLAYER_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    //base health should be current player health
    HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, CalculateCurrentHP(player.current_hp, 0), CalculateCurrentMaxHP(player.max_hp, 0));

    bool fireballHitEnemy = AreCirclesIntersecting(fireball_x, PLAYER_Y_AXIS, 90 / 2, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH / 2);

    if (!fireballHitEnemy) {
        fireball_x += 10;
        CP_Image_Draw(fireball, fireball_x, PLAYER_Y_AXIS, 90, 90, 255);
        CP_Image_Draw(player_opened, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
        HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, 0), CalculateCurrentMaxHP(dumpling.max_hp, 0));
    } else {
        CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
        //base health should be current enemy health, and base attack should be current enemy attack
        HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, dumpling.attack), CalculateCurrentMaxHP(dumpling.max_hp, 0));
    }

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(ENEMY_X_AXIS + 25, ENEMY_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    
    CP_Image_Draw(dumpling_closed, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
}

void Game_Exit(void)
{
    CP_Image_Free(player_opened);
    CP_Image_Free(player_closed);
    CP_Image_Free(dumpling_closed);
    CP_Image_Free(dumpling_opened);
}