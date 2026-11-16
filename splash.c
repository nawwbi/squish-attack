#include "cprocessing.h"
#include "splash.h"
#include "mainmenu.h"

CP_Image splash_digipen_logo;
CP_Image splash_team_logo;
CP_Font font;

float splash_timer = 0.0f;

void Splash_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    // load assets
    splash_digipen_logo = CP_Image_Load("./Assets/digipen-logo-red.png");

    // team logo
    splash_team_logo = CP_Image_Load("./Assets/moonsstudio-logo.png");

    font = CP_Font_Load("./Assets/Jersey15-Regular.ttf");

    // start timer from 0
    splash_timer = 0.0f;
}

void Splash_Update(void)
{
    float dt = CP_System_GetDt();

    // add time every frame
    splash_timer += dt;

    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    // first 2 seconds, DigiPen logo and copyright
    if (splash_timer < 2.0f)
    {
        CP_Image_Draw(splash_digipen_logo,800, 400, 500, 170, 255);
        CP_Font_Set(font);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(24);

        CP_Font_DrawText("All content © 2026 DigiPen Institute of Technology Singapore. All Rights Reserved", 800.0f, 550.0f);
    }

    // next 2 seconds, team logo
    else if (splash_timer < 4.0f)
    {
        CP_Image_Draw(splash_team_logo, 800, 450, 500, 300, 255);
    }

    // after 4 seconds, main menu
    else
    {
        CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
}

void Splash_Exit(void)
{
    CP_Image_Free(splash_digipen_logo);
    CP_Image_Free(splash_team_logo);
}