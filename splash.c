#include "cprocessing.h"
#include "splash.h"
#include "mainmenu.h"
#include "utils.h"

CP_Image splash_digipen_logo;
CP_Image splash_team_logo;
CP_Image splash_skip_button;
CP_Font font;

float splash_timer = 0.0f;

void Splash_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    splash_digipen_logo = CP_Image_Load("./Assets/digipen-logo-red.png");

    // team logo
    splash_team_logo = CP_Image_Load("./Assets/moonsstudios-logo.png");

    splash_skip_button = CP_Image_Load("./Assets/game-button.png");;

    font = CP_Font_Load("./Assets/Jersey15-Regular.ttf");

    // start timer from 0
    splash_timer = 0.0f;
}

void Splash_Update(void)
{
    float dt = CP_System_GetDt();
    int alpha = 255;

    // add time every frame
    splash_timer += dt;

    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    // DigiPen logo fade in
    if (splash_timer < 0.5f)
    {
        alpha = (int)(splash_timer / 0.5f * 255);
        
        CP_Image_Draw(splash_digipen_logo,800, 400, 500, 170, alpha);

        CP_Font_Set(font);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
        CP_Settings_TextSize(24);

        CP_Font_DrawText("All content © 2026 DigiPen Institute of Technology Singapore. All Rights Reserved", 800.0f, 550.0f);
    }

    // DigiPen logo, fully visible
    else if (splash_timer < 2.5f)
    {
        CP_Image_Draw(splash_digipen_logo, 800, 400, 500, 170, 255);

        CP_Font_Set(font);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(24);
        CP_Font_DrawText("All content © 2026 DigiPen Institute of Technology Singapore. All Rights Reserved", 800.0f, 550.0f);
    }

    // DigiPen logo fade out
    else if (splash_timer < 3.0f)
    {
        alpha = (int)((3.0f - splash_timer) / 0.5f * 255);

        CP_Image_Draw(splash_digipen_logo, 800, 400, 500, 170, alpha);

        CP_Font_Set(font);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
        CP_Settings_TextSize(24);
        CP_Font_DrawText("All content © 2026 DigiPen Institute of Technology Singapore. All Rights Reserved", 800.0f, 550.0f);
    }

    // then, team logo fade in
    else if (splash_timer < 3.5f)
    {
        alpha = (int)((splash_timer - 3.0f) / 0.5f * 255);
        
        CP_Image_Draw(splash_team_logo, 800, 450, 600, 300, alpha);
    }

    // team logo fully visible
    else if (splash_timer < 5.5f)
    {
        CP_Image_Draw(splash_team_logo, 800, 450, 600, 300, 255);
    }

    // team logo fade out
    else if (splash_timer < 6.0f)
    {
        alpha = (int)((6.0f - splash_timer) / 0.5f * 255);

        CP_Image_Draw(splash_team_logo, 800, 450, 600, 300, alpha);
    }

    // main menu
    else
    {
        CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }

    // skip button
    CP_Image_Draw(splash_skip_button, 1450, 820, 220, 200, 255);

    // skip text
    CP_Font_Set(font);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(35);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("Skip", 1450.0f, 810.0f);

    // skip button click
    int skip_clicked = Is_Area_Clicked(1450.0f, 820.0f, 220.0f, 80.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

    if (skip_clicked == 1)
    {
        CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
}

void Splash_Exit(void)
{
    CP_Image_Free(splash_digipen_logo);
    CP_Image_Free(splash_team_logo);
    CP_Image_Free(splash_skip_button);
}