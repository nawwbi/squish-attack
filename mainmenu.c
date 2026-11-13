#include "cprocessing.h"
#include "game.h"
#include "utils.h"
#include "mainmenu.h"
#include "upgrades.h"
#include "constants.h"
#include "controls.h"
#include "credits.h"

CP_Font font;
CP_Image background;
CP_Image banner;
CP_Image game_button;

bool main_confirm_quit = false;

void Main_Menu_Init(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);

	font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	background = CP_Image_Load("./Assets/background.png");
	banner = CP_Image_Load("./Assets/title-banner1.png");
	game_button = CP_Image_Load("./Assets/game_button.png");

	// confirmation popup starts closed
	main_confirm_quit = false;
}

void Main_Menu_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));

	CP_Settings_Tint(CP_Color_Create(0, 0, 0, 100));
	CP_Image_Draw(background, 800, 450, 1600, 900, 255);
	CP_Settings_NoTint();
	CP_Image_Draw(banner, 800, 450, 1600, 900, 255);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(font);

	CP_Settings_TextSize(60);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText("Squish Attack!", 800.0f, 70.0f);

	CP_Image_Draw(game_button, 800, 300, 350, 360, 255); // play button
	CP_Image_Draw(game_button, 800, 415, 350, 360, 255); // controls button
	CP_Image_Draw(game_button, 800, 530, 350, 360, 255); // credits button
	CP_Image_Draw(game_button, 800, 645, 350, 360, 255); // quit button

	CP_Settings_TextSize(60);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	CP_Font_DrawText("Play", 800.0f, 285.0f);
	CP_Font_DrawText("Controls", 800.0f, 400.0f);
	CP_Font_DrawText("Credits", 800.0f, 515.0f);
	CP_Font_DrawText("Quit", 800.0f, 630.0f);

    // normal menu buttons only work when confirmation popup is closed
    if (main_confirm_quit == false)
    {
        int playClicked = IsAreaClicked(800.0f, 300.0f, 300.0f, 80.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (playClicked == 1)
        {
            CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
        }

        int controlsClicked = IsAreaClicked(800.0f, 415.0f, 300.0f, 80.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (controlsClicked == 1)
        {
            CP_Engine_SetNextGameState(Controls_Init, Controls_Update, Controls_Exit);
        }

        int creditsClicked = IsAreaClicked(800.0f, 530.0f, 300.0f, 80.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (creditsClicked == 1)
        {
            CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit);
        }

        int quitClicked = IsAreaClicked(800.0f, 645.0f, 300.0f, 80.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (quitClicked == 1)
        {
            main_confirm_quit = true;
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
        CP_Image_Draw(game_button, 680, 500, 250, 260, 255);
        CP_Image_Draw(game_button, 920, 500, 250, 260, 255);

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

        // no closes popup and stays on main menu
        if (noClicked == 1)
        {
            main_confirm_quit = false;
        }
    }
}

void Main_Menu_Exit(void)
{

}