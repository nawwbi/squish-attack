#include "cprocessing.h"
#include "credits.h"
#include "mainmenu.h"
#include "utils.h"

CP_Font credits_font;
CP_Image credits_background;
CP_Image credits_banner;
CP_Image credits_button;

CP_Image credits_logo;
float credits_y; // starting y coordinate for scrolling

void Credits_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    credits_font = CP_Font_Load("Assets/Exo2-Regular.ttf");
    credits_background = CP_Image_Load("./Assets/background.png");
    credits_banner = CP_Image_Load("./Assets/title-banner1.png");
    credits_button = CP_Image_Load("./Assets/game_button.png");

    credits_logo = CP_Image_Load("./Assets/digipen-logo-red.png");
    credits_y = 680.0f;
}

void Credits_Update(void)
{
    CP_Image_Draw(credits_background, 800, 450, 1600, 900, 255);
    CP_Image_Draw(credits_banner, 800, 450, 1600, 900, 255);

    CP_Image_Draw(credits_logo, 110, 120, 160, 60, 255);

    DrawRectangle(CP_Color_Create(0, 0, 0, 170), 800, 430, 700, 560, 0);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    CP_Font_Set(credits_font);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    CP_Settings_TextSize(60);
    CP_Font_DrawText("Credits", 800.0f, 70.0f);

    CP_Settings_TextSize(18);

    if (credits_y >= 210.0f && credits_y <= 670.0f)
    {
        CP_Font_DrawText("All content © 2026 DigiPen Institute of Technology Singapore.", 800.0f, credits_y);
    }

    if (credits_y + 30 >= 210.0f && credits_y + 30 <= 670.0f)
    {
        CP_Font_DrawText("All Rights Reserved.", 800.0f, credits_y + 30);
    }
    
    CP_Settings_TextSize(30);
    if (credits_y + 90 >= 210.0f && credits_y + 90 <= 670.0f)
    {
        CP_Font_DrawText("TEAM MEMBERS", 800.0f, credits_y + 90);
    }

    CP_Settings_TextSize(24);
    if (credits_y + 135 >= 210.0f && credits_y + 135 <= 670.0f)
    {
        CP_Font_DrawText("Nabilah Binte Jumahat", 800.0f, credits_y + 135);
    }
    if (credits_y + 170 >= 210.0f && credits_y + 170 <= 670.0f)
    {
        CP_Font_DrawText("Fang Rong Cheng", 800.0f, credits_y + 170);
    }
    if (credits_y + 205 >= 210.0f && credits_y + 205 <= 670.0f)
    {
        CP_Font_DrawText("Nikita Kumari D/O Sivakumar", 800.0f, credits_y + 205);
    }
    if (credits_y + 240 >= 210.0f && credits_y + 240 <= 670.0f)
    {
        CP_Font_DrawText("Michelle Stefanie Djunaedi", 800.0f, credits_y + 240);
    }

    CP_Settings_TextSize(30);
    if (credits_y + 295 >= 210.0f && credits_y + 295 <= 670.0f)
    {
        CP_Font_DrawText("INSTRUCTORS", 800.0f, credits_y + 295);
    }

    CP_Settings_TextSize(21);
    if (credits_y + 330 >= 210.0f && credits_y + 330 <= 670.0f) 
    {
        CP_Font_DrawText("Dr. Soroor Malekmohammadi Faradounbeh", 800.0f, credits_y + 330);
    }
    if (credits_y + 360 >= 210.0f && credits_y + 360 <= 670.0f)
    {
        CP_Font_DrawText("Wong Han Feng, Gerald", 800.0f, credits_y + 360);
    }
    if (credits_y + 390 >= 210.0f && credits_y + 390 <= 670.0f)
    {
        CP_Font_DrawText("Tan Chee Wei Tommy", 800.0f, credits_y + 390);
    }

    CP_Settings_TextSize(19);
    if (credits_y + 435 >= 210.0f && credits_y + 435 <= 670.0f)
    {
        CP_Font_DrawText("President: Claude Comair", 800.0f, credits_y + 435);
    }
    if (credits_y + 475 >= 210.0f && credits_y + 475 <= 670.0f)
    {
        CP_Font_DrawText("Executives: CHU Jason Yeu Tat, Michael GATS, TAN Chek Ming,", 800.0f, credits_y + 475);
    }
    if (credits_y + 505 >= 210.0f && credits_y + 505 <= 670.0f)
    {
        CP_Font_DrawText("Prasanna Kumar GHALI, Mandy WONG, Johnny DEEK", 800.0f, credits_y + 505);
    }

    // update scrolling credits
    credits_y -= 1.0f;

    if (credits_y < -320.0f)
    {
        credits_y = 680.0f;
    }

    CP_Image_Draw(credits_button, 250, 750, 350, 360, 255);

    CP_Settings_TextSize(60);
    CP_Font_DrawText("Back", 250.0f, 735.0f);

    int backClicked = IsAreaClicked(250.0f, 750.0f, 400.0f, 120.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

    if (backClicked == 1) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); }
}

void Credits_Exit(void)
{
}