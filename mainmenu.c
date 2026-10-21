#include "cprocessing.h"
#include "game.h"
#include "utils.h"

CP_Font font;

void Main_Menu_Init(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);

	font = CP_Font_Load("Assets/Exo2-Regular.ttf");
}

void Main_Menu_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));

	CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(80);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(font);

	CP_Font_DrawText("Play", 800.0f, 350.0f);
	CP_Font_DrawText("Exit", 800.0f, 550.0f);

	int playClicked = IsAreaClicked(800.0f, 350.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());
	if (playClicked == 1) { CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit); }

	int exitClicked = IsAreaClicked(800.0f, 550.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());
	if (exitClicked == 1) { CP_Engine_Terminate(); }
}

void Main_Menu_Exit(void)
{

}