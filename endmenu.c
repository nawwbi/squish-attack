#include "cprocessing.h"
#include "game.h"
#include "utils.h"
#include "endmenu.h"
#include "constants.h"
#include <stdio.h>

CP_Font end_font;
CP_Image end_background;
CP_Image end_banner;
CP_Image end_button;

bool end_confirm_quit = false;

void End_Menu_Init(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);

	end_font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	end_background = CP_Image_Load("./Assets/background.png");
	end_banner = CP_Image_Load("./Assets/title-banner1.png");
	end_button = CP_Image_Load("./Assets/game_button.png");

	// confirmation popup starts closed
	end_confirm_quit = false;
}

void End_Menu_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));

	CP_Settings_Tint(CP_Color_Create(0, 0, 0, 100));
	CP_Image_Draw(end_background, 800, 450, 1600, 900, 255);
	CP_Settings_NoTint();

	CP_Image_Draw(end_banner, 800, 450, 1600, 900, 255);

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(end_font);

	CP_Settings_TextSize(60);
	CP_Font_DrawText("Game Complete", 800.0f, 70.0f);

	CP_Settings_TextSize(100);
	CP_Font_DrawText("You Win!", 800.0f, 260.0f);

	CP_Settings_TextSize(40);
	char score_val[50];
	snprintf(score_val,50, "Score: %d", score);
	CP_Font_DrawText(score_val, 800.0f, 330.0f);

	CP_Settings_TextSize(40);
	char highscore_val[50];
	snprintf(highscore_val, 50, "High Score: %d", highscore);
	CP_Font_DrawText(highscore_val, 800.0f, 370.0f);


	CP_Image_Draw(end_button, 800, 480, 350, 360, 255); // play again button
	CP_Image_Draw(end_button, 800, 600, 350, 360, 255); // quit button

	CP_Settings_TextSize(50);
	CP_Font_DrawText("Play Again", 800.0f, 465.0f);
	CP_Font_DrawText("Quit", 800.0f, 585.0f);

    // normal buttons only work when confirmation popup is closed
    if (end_confirm_quit == false)
    {
        int playClicked = IsAreaClicked(800.0f, 480.0f, 400.0f, 120.0f,
            CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (playClicked == 1)
        {
            CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
        }

        int quitClicked = IsAreaClicked(800.0f, 600.0f, 400.0f, 120.0f,
            CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (quitClicked == 1)
        {
            end_confirm_quit = true;
        }
    }

    // confirmation popup
    else
    {
        DrawRectangle(CP_Color_Create(0, 0, 0, 220),
            800, 450, 600, 300, 0);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(45);
        CP_Font_DrawText("Are you sure you want to quit?", 800.0f, 370.0f);

        // yes and no buttons
        CP_Image_Draw(end_button, 680, 500, 250, 260, 255);
        CP_Image_Draw(end_button, 920, 500, 250, 260, 255);

        CP_Settings_TextSize(40);
        CP_Font_DrawText("Yes", 680.0f, 490.0f);
        CP_Font_DrawText("No", 920.0f, 490.0f);

        int yesClicked = IsAreaClicked(680.0f, 500.0f, 200.0f, 70.0f,
            CP_Input_GetMouseX(), CP_Input_GetMouseY());

        int noClicked = IsAreaClicked(920.0f, 500.0f, 200.0f, 70.0f,
            CP_Input_GetMouseX(), CP_Input_GetMouseY());

        // yes closes the application
        if (yesClicked == 1)
        {
            CP_Engine_Terminate();
        }

        // no closes popup and stays on win screen
        if (noClicked == 1)
        {
            end_confirm_quit = false;
        }
    }
}

void End_Menu_Exit(void)
{

}