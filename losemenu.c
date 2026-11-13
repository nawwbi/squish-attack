#include "cprocessing.h"
#include "game.h"
#include "utils.h"
#include "losemenu.h"
#include "constants.h"
#include <stdio.h>

CP_Font lose_font;
CP_Image lose_background;
CP_Image lose_banner;
CP_Image lose_button;

bool lose_confirm_quit = false;

void Lose_Menu_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    lose_font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    lose_background = CP_Image_Load("./Assets/background.png");
    lose_banner = CP_Image_Load("./Assets/title-banner1.png");
    lose_button = CP_Image_Load("./Assets/game_button.png");

    // confirmation popup starts closed
    lose_confirm_quit = false;
}

void Lose_Menu_Update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));

    // background
    CP_Settings_Tint(CP_Color_Create(0, 0, 0, 100));
    CP_Image_Draw(lose_background, 800, 450, 1600, 900, 255);
    CP_Settings_NoTint();

    CP_Image_Draw(lose_banner, 800, 450, 1600, 900, 255);

    // text settings
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_Set(lose_font);

    // title
    CP_Settings_TextSize(60);
    CP_Font_DrawText("Game Over", 800.0f, 70.0f);

    CP_Settings_TextSize(100);
    CP_Font_DrawText("You Lose!", 800.0f, 260.0f);

    // score
    CP_Settings_TextSize(40);
    char score_val[50];
    snprintf(score_val, 50, "Score: %d", score);
    CP_Font_DrawText(score_val, 800.0f, 330.0f);

    // high score
    char highscore_val[50];
    snprintf(highscore_val, 50, "High Score: %d", highscore);
    CP_Font_DrawText(highscore_val, 800.0f, 370.0f);

    // play again and quit buttons
    CP_Image_Draw(lose_button, 800, 480, 350, 360, 255);
    CP_Image_Draw(lose_button, 800, 600, 350, 360, 255);

    CP_Settings_TextSize(50);
    CP_Font_DrawText("Play Again", 800.0f, 465.0f);
    CP_Font_DrawText("Quit", 800.0f, 585.0f);

    // normal buttons only work when confirmation popup is closed
    if (lose_confirm_quit == false)
    {
        int playClicked = IsAreaClicked(800.0f, 480.0f, 400.0f, 120.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (playClicked == 1)
        {
            CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
        }

        int quitClicked = IsAreaClicked(800.0f, 600.0f, 400.0f, 120.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (quitClicked == 1)
        {
            lose_confirm_quit = true;
        }
    }

    // confirmation popup
    else
    {
        DrawRectangle(CP_Color_Create(0, 0, 0, 220), 800, 450, 600, 300, 0);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(45);
        CP_Font_DrawText("Are you sure you want to quit?", 800.0f, 370.0f);

        // yes and no buttons
        CP_Image_Draw(lose_button, 680, 500, 250, 260, 255);
        CP_Image_Draw(lose_button, 920, 500, 250, 260, 255);

        CP_Settings_TextSize(40);
        CP_Font_DrawText("Yes", 680.0f, 490.0f);
        CP_Font_DrawText("No", 920.0f, 490.0f);

        int yesClicked = IsAreaClicked(680.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        int noClicked = IsAreaClicked(920.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        // yes closes the application
        if (yesClicked == 1)
        {
            CP_Engine_Terminate();
        }

        // no closes the popup and stays on lose screen
        if (noClicked == 1)
        {
            lose_confirm_quit = false;
        }
    }
}

void Lose_Menu_Exit(void)
{
}