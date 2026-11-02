#include "cprocessing.h"
#include "game.h"
#include "utils.h"
#include "mainmenu.h"
#include "upgrades.h"
#include "constants.h"

CP_Font font;
CP_Image background;
CP_Image banner;
CP_Image game_button;

void Main_Menu_Init(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);

	font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	background = CP_Image_Load("./Assets/background.png");
	banner = CP_Image_Load("./Assets/title-banner1.png");
	game_button = CP_Image_Load("./Assets/game_button.png");
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

	CP_Image_Draw(game_button, 800, 340, 350, 360, 255); // play button
	CP_Image_Draw(game_button, 800, 480, 350, 360, 255); // controls button
	CP_Image_Draw(game_button, 800, 620, 350, 360, 255); // quit button

	CP_Settings_TextSize(60);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText("Play", 800.0f, 325.0f);
	CP_Font_DrawText("Controls", 800.0f, 465.0f);
	CP_Font_DrawText("Quit", 800.0f, 605.0f);

	int playClicked = IsAreaClicked(800.0f, 350.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());
	if (playClicked == 1) { CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit); }

	int exitClicked = IsAreaClicked(800.0f, 550.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());
	if (exitClicked == 1) { CP_Engine_Terminate(); }
}

void Main_Menu_Exit(void)
{

}