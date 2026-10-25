#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "game.h"
#include "constants.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image player = NULL;
CP_Image dumpling = NULL;
CP_Image background = NULL;

void Game_Init(void)
{
	player = CP_Image_Load("./Assets/player-closed.png");
	dumpling = CP_Image_Load("./Assets/dumpling-closed.png");
	background = CP_Image_Load("./Assets/background.png");
}

void Game_Update(void)
{
    CP_Color red = CP_Color_Create(255, 200, 200, 100);
    CP_Color darkRed = CP_Color_Create(255, 0, 0, 255);

    CP_Color green = CP_Color_Create(200, 255, 200, 100);
    CP_Color darkGreen = CP_Color_Create(0, 255, 0, 255);
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    int width = CP_System_GetWindowWidth();
    int height = CP_System_GetWindowHeight();

    CP_Image_Draw(background, 800, 450, 1600, 900, 255);

    HealthBar(darkGreen, green, 230, 430, 2000, 2222);
    CP_Image_Draw(player, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);

    HealthBar(darkRed, red, 1250, 500, 500, 5000);
    CP_Image_Draw(dumpling, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
}

void Game_Exit(void)
{
    CP_Image_Free(player);
}