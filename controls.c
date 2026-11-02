#include "cprocessing.h"
#include "controls.h"
#include "mainmenu.h"
#include "utils.h"

CP_Font controls_font;
CP_Image controls_background;
CP_Image controls_banner;
CP_Image controls_button;

void Controls_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    controls_font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    controls_background = CP_Image_Load("./Assets/background.png");
    controls_banner = CP_Image_Load("./Assets/title-banner1.png");
    controls_button = CP_Image_Load("./Assets/game_button.png");
}

void Controls_Update(void)
{
    CP_Image_Draw(controls_background, 800, 450, 1600, 900, 255);
    CP_Image_Draw(controls_banner, 800, 450, 1600, 900, 255);

    DrawRectangle(CP_Color_Create(0, 0, 0, 150), 800, 420, 700, 500, 0);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_Set(controls_font);

    CP_Settings_TextSize(60);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText("Game Summary", 800.0f, 70.0f);

    CP_Settings_TextSize(40);
    CP_Font_DrawText("HOW TO PLAY", 800.0f, 210.0f);

    CP_Settings_TextSize(25);
    CP_Font_DrawText("Choose one of three upgrades before combat begins.", 800.0f, 270.0f);
    CP_Font_DrawText("Your character attacks automatically.", 800.0f, 320.0f);
    CP_Font_DrawText("Enemies will counterattack.", 800.0f, 355.0f);
    CP_Font_DrawText("Defeat all enemies within 10 rounds to progress.", 800.0f, 405.0f);
    CP_Font_DrawText("Clear all levels and defeat the boss in the final level to win!", 800.0f, 455.0f);

    CP_Settings_TextSize(30);
    CP_Font_DrawText("CONTROLS", 800.0f, 515.0f);

    CP_Settings_TextSize(25);
    CP_Font_DrawText("LMB - Select upgrades", 800.0f, 555.0f);
    CP_Font_DrawText("LMB - Navigate buttons", 800.0f, 590.0f);

    CP_Image_Draw(controls_button, 250, 750, 350, 360, 255);

    CP_Settings_TextSize(60);
    CP_Font_DrawText("Back", 250.0f, 735.0f);

    int backClicked = IsAreaClicked(250.0f, 750.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());
    if (backClicked == 1) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); }
}

void Controls_Exit(void)
{
}