#include "cprocessing.h"
#include "settingscontrols.h"
#include "controls.h"
#include "pause.h"
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
CP_Image controls_skills;

int settings_controls_page = 1;
extern bool is_controls;

void Settings_Controls_Init(void)
{
    CP_Settings_RectMode(CP_POSITION_CENTER);

    controls_background = CP_Image_Load("./Assets/background.png");
    controls_banner = CP_Image_Load("./Assets/title-banner1.png");
    controls_button = CP_Image_Load("./Assets/game-button.png");

    controls_left_arrow = CP_Image_Load("./Assets/skip-button-left.png");
    controls_right_arrow = CP_Image_Load("./Assets/skip-button.png");

    controls_gameplay = CP_Image_Load("./Assets/controls-gameplay.png");
    controls_speed = CP_Image_Load("./Assets/controls-speed.png");
    controls_upgrades = CP_Image_Load("./Assets/controls-upgrades.png");

    controls_skills = CP_Image_Load("./Assets/controls-skills.png");

    settings_controls_page = 1;
    is_controls = true;

}

void Settings_Controls_Update(void)
{
    if (is_controls == true) {

        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(controls_background, 800, 450, 1600, 900, 255);
        CP_Image_Draw(controls_banner, 800, 450, 1600, 900, 255);

        // black background rectangle
        Draw_Rectangle(CP_Color_Create(0, 0, 0, 150), 800, 420, 850, 500, 0);

        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

        // title
        CP_Settings_TextSize(60);
        CP_Font_DrawText("How to Play", 800.0f, 70.0f);

        // page 1: auto combat
        if (settings_controls_page == 1)
        {
            CP_Image_Draw(controls_gameplay, 800, 350, 650, 350, 255);

            CP_Settings_TextSize(25);
            CP_Font_DrawText("Combat happens automatically.", 800.0f, 555.0f);
            CP_Font_DrawText("Defeat the enemy within 10 rounds to progress.", 800.0f, 590.0f);
        }

        // page 2: speed button
        else if (settings_controls_page == 2)
        {
            CP_Image_Draw(controls_speed, 800, 350, 650, 350, 255);

            CP_Settings_TextSize(25);
            CP_Font_DrawText("Click the speed button to speed up combat.", 800.0f, 555.0f);
            CP_Font_DrawText("You can increase the battle speed up to x3.", 800.0f, 590.0f);
        }

        // page 3: skills/upgrades
        else if (settings_controls_page == 3)
        {
            CP_Image_Draw(controls_upgrades, 800, 350, 650, 350, 255);

            CP_Settings_TextSize(25);
            CP_Font_DrawText("Click to choose 1 of the 3 upgrades shown.", 800.0f, 555.0f);
            CP_Font_DrawText("Your chosen upgrade stacks until the end of the game.", 800.0f, 590.0f);
        }

        // page 4: player skills
        else if (settings_controls_page == 4)
        {
            CP_Image_Draw(controls_skills, 800, 350, 650, 350, 255);

            CP_Settings_TextSize(25);
            CP_Font_DrawText("Use mana to activate skills during combat.", 800.0f, 555.0f);
            CP_Font_DrawText("Each skill has a different mana cost and effect.", 800.0f, 590.0f);
        }

        // page number
        CP_Settings_TextSize(22);

        if (settings_controls_page == 1)
        {
            CP_Font_DrawText("Page 1 / 4", 800.0f, 635.0f);
        }
        else if (settings_controls_page == 2)
        {
            CP_Font_DrawText("Page 2 / 4", 800.0f, 635.0f);
        }
        else if (settings_controls_page == 3)
        {
            CP_Font_DrawText("Page 3 / 4", 800.0f, 635.0f);
        }
        else if (settings_controls_page == 4)
        {
            CP_Font_DrawText("Page 4 / 4", 800.0f, 635.0f);
        }


        // left and right page toggle buttons
        CP_Image_Draw(controls_left_arrow, 375, 420, 100, 100, 255);
        CP_Image_Draw(controls_right_arrow, 1225, 420, 100, 100, 255);


        // check left and right page buttons
        int left_clicked = Is_Area_Clicked(375.0f, 420.0f, 100.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        int right_clicked = Is_Area_Clicked(1225.0f, 420.0f, 100.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());


        // go to previous page
        if (left_clicked == 1)
        {
            Audio_Play_Button_Click();

            settings_controls_page--;

            if (settings_controls_page < 1)
            {
                settings_controls_page = 4;
            }
        }

        // go to next page
        if (right_clicked == 1)
        {
            Audio_Play_Button_Click();

            settings_controls_page++;

            if (settings_controls_page > 4)
            {
                settings_controls_page = 1;
            }
        }


        // back button
        CP_Image_Draw(controls_button, 250, 750, 350, 360, 255);

        CP_Settings_TextSize(60);
        CP_Font_DrawText("Back", 250.0f, 735.0f);

        int backClicked = Is_Area_Clicked(250.0f, 750.0f, 400.0f, 150.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (backClicked == 1)
        {
            Audio_Play_Button_Click();
            is_controls = false;
        }
    }
}

void Settings_Controls_Exit(void)
{
}