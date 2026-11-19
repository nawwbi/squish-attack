#include "cprocessing.h"
#include "controls.h"
#include "mainmenu.h"
#include "utils.h"
#include "audio.h"

CP_Image controls_background;
CP_Image controls_banner;
CP_Image controls_button;
CP_Image controls_left_arrow;
CP_Image controls_right_arrow;
CP_Image controls_gameplay;
CP_Image controls_speed;
CP_Image controls_upgrades;

int controls_page = 1;

void Controls_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    controls_background = CP_Image_Load("./Assets/background.png");
    controls_banner = CP_Image_Load("./Assets/title-banner1.png");
    controls_button = CP_Image_Load("./Assets/game_button.png");

    controls_left_arrow = CP_Image_Load("./Assets/skip-button-left.png");
    controls_right_arrow = CP_Image_Load("./Assets/skip-button.png");

    controls_gameplay = CP_Image_Load("./Assets/controls-gameplay.png");
    controls_speed = CP_Image_Load("./Assets/controls-speed.png");
    controls_upgrades = CP_Image_Load("./Assets/controls-upgrades.png");

    controls_page = 1;

}

void Controls_Update(void)
{
    CP_Image_Draw(controls_background, 800, 450, 1600, 900, 255);
    CP_Image_Draw(controls_banner, 800, 450, 1600, 900, 255);

    // black background rectangle
    DrawRectangle(CP_Color_Create(0, 0, 0, 150), 800, 420, 850, 500, 0);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    // title
    CP_Settings_TextSize(60);
    CP_Font_DrawText("How to Play", 800.0f, 70.0f);

    // page 1: auto combat
    if (controls_page == 1)
    {
        CP_Image_Draw(controls_gameplay, 800, 350, 650, 325, 255);

        CP_Settings_TextSize(25);
        CP_Font_DrawText("Combat happens automatically.", 800.0f, 555.0f);
        CP_Font_DrawText("Defeat the enemy within 10 rounds to progress.", 800.0f, 590.0f);
    }

    // page 2: speed button
    else if (controls_page == 2)
    {
        CP_Image_Draw(controls_speed, 800, 350, 650, 325, 255);

        CP_Settings_TextSize(25);
        CP_Font_DrawText("Click the speed button to speed up combat.", 800.0f, 555.0f);
        CP_Font_DrawText("You can increase the battle speed up to x3.", 800.0f, 590.0f);
    }

    // page 3: skills/upgrades
    else if (controls_page == 3)
    {
        CP_Image_Draw(controls_upgrades, 800, 350, 650, 325, 255);

        CP_Settings_TextSize(25);
        CP_Font_DrawText("Click to choose 1 of the 3 upgrades shown.", 800.0f, 555.0f);
        CP_Font_DrawText("Your chosen upgrade stacks until the end of the game.", 800.0f, 590.0f);
    }


    // page number
    CP_Settings_TextSize(22);

    if (controls_page == 1)
    {
        CP_Font_DrawText("Page 1 / 3", 800.0f, 635.0f);
    }
    else if (controls_page == 2)
    {
        CP_Font_DrawText("Page 2 / 3", 800.0f, 635.0f);
    }
    else if (controls_page == 3)
    {
        CP_Font_DrawText("Page 3 / 3", 800.0f, 635.0f);
    }


    // left and right page toggle buttons
    CP_Image_Draw(controls_left_arrow, 375, 420, 100, 100, 255);
    CP_Image_Draw(controls_right_arrow, 1225, 420, 100, 100, 255);


    // check left and right page buttons
    int leftClicked = IsAreaClicked(375.0f, 420.0f, 100.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

    int rightClicked = IsAreaClicked(1225.0f, 420.0f, 100.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());


    // go to previous page
    if (leftClicked == 1)
    {
        Audio_Play_Button_Click();

        controls_page--;

        if (controls_page < 1)
        {
            controls_page = 3;
        }
    }

    // go to next page
    if (rightClicked == 1)
    {
        Audio_Play_Button_Click();

        controls_page++;

        if (controls_page > 3)
        {
            controls_page = 1;
        }
    }


    // back button
    CP_Image_Draw(controls_button, 250, 750, 350, 360, 255);

    CP_Settings_TextSize(60);
    CP_Font_DrawText("Back", 250.0f, 735.0f);

    int backClicked = IsAreaClicked(250.0f, 750.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

    if (backClicked == 1)
    {
        Audio_Play_Button_Click();
        CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
}


void Controls_Exit(void)
{
}