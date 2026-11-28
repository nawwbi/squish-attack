#include "cprocessing.h"
#include "math.h"
#include "ui.h"
#include "combat.h"
#include "game.h"
#include "mainmenu.h"
#include "pause.h"
#include "audio.h"
#include "settings.h"
#include <stdbool.h>

CP_Image
menu_button;

CP_Color
white,
green;

extern int is_paused;
extern bool is_settings;

// audio states
float main_volume = 1.0f; // 1.0f = 100%
bool bgm_on = true;
bool sfx_on = true;

void Update_Audio(void) {
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, bgm_on ? main_volume : 0.0f);
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, sfx_on ? main_volume : 0.0f);
}
void Settings_Init(void) {
    CP_Settings_RectMode(CP_POSITION_CENTER);
	menu_button = CP_Image_Load("./Assets/game-button.png");
    white = CP_Color_Create(255, 255, 255, 255);
    green = CP_Color_Create(100, 255, 100, 255);

    Update_Audio();
}

void Settings_Update(void) {
    if (is_settings == true) {
            Draw_Rectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 600, 700, 20);

            CP_Settings_Fill(white);
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("SETTINGS", 800, 150);

            // toggle audio volume buttons -> 0% 25% 50% 75% 100% 1400
            CP_Settings_Fill(white);
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("VOLUME", 800, 240);

            // volume 0%
            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(50);
            CP_Font_DrawText("0", 540, 320);

            Draw_Rectangle(green, 610, 320, 50, 40, 0);
            if (Is_Area_Clicked(610, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
            {
                Audio_Play_Button_Click();
                main_volume = 0.0f;
                Update_Audio();
            }

            // volume 25%
            if (Is_Area_Clicked(700, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
            {
                Audio_Play_Button_Click();
                main_volume = 0.25f;
                Update_Audio();
            }

            // volume 50%
            if (Is_Area_Clicked(790, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
            {
                Audio_Play_Button_Click();
                main_volume = 0.50f;
                Update_Audio();
            }

            // volume 75%
            if (Is_Area_Clicked(880, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
            {
                Audio_Play_Button_Click();
                main_volume = 0.75f;
                Update_Audio();
            }

            // volume 100%
            CP_Settings_Fill(white);
            CP_Settings_TextSize(50);
            CP_Font_DrawText("100", 1040, 320);

            if (Is_Area_Clicked(970, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
            {
                Audio_Play_Button_Click();
                main_volume = 1.0f;
                Update_Audio();
            }

            // green volume boxes
            // green box 25%
            if (main_volume >= 0.25f) {
                Draw_Rectangle(green, 700, 320, 50, 40, 0);
            }
            else {
                Draw_Rectangle(white, 700, 320, 50, 40, 0);
            }

            // green box 50%
            if (main_volume >= 0.50f) {
                Draw_Rectangle(green, 790, 320, 50, 40, 0);
            }
            else {
                Draw_Rectangle(white, 790, 320, 50, 40, 0);
            }

            // green box 75%
            if (main_volume >= 0.75f) {
                Draw_Rectangle(green, 880, 320, 50, 40, 0);
            }
            else {
                Draw_Rectangle(white, 880, 320, 50, 40, 0);
            }

            // green box 100%
            if (main_volume >= 1.0f) {
                Draw_Rectangle(green, 970, 320, 50, 40, 0);
            }
            else {
                Draw_Rectangle(white, 970, 320, 50, 40, 0);
            }

            CP_Settings_Fill(white); // reset text fill color back to white after drawing green volume boxes

            // BGM and SFX settings text
            CP_Settings_Fill(white);
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("BGM & SFX", 800, 420);

            // background music on / off
            CP_Settings_RectMode(CP_POSITION_CENTER);
            CP_Image_Draw(menu_button, 800, 490, 350, 220, 255);
            CP_Settings_TextSize(25.0f);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText(bgm_on ? "Background Music: On" : "Background Music: Off", 800, 485);

            if (Is_Area_Clicked(800, 490, 350, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                bgm_on = !bgm_on;
                Update_Audio();
                Audio_Play_Button_Click();
            }

            // sound effect on / off
            CP_Settings_RectMode(CP_POSITION_CENTER);
            CP_Image_Draw(menu_button, 800, 570, 350, 220, 255);
            CP_Settings_TextSize(25.0f);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText(sfx_on ? "Sound Effects: On" : "Sound Effects: Off", 800, 565);

            if (Is_Area_Clicked(800, 570, 350, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                sfx_on = !sfx_on;
                Update_Audio();
                Audio_Play_Button_Click();
            }

            // back button
            CP_Settings_RectMode(CP_POSITION_CENTER);
            CP_Image_Draw(menu_button, 800, 690, 350, 220, 255);
            CP_Settings_TextSize(25.0f);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("Back", 800, 680);

            if (Is_Area_Clicked(800, 690, 350, 220, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                Audio_Play_Button_Click();
                is_settings = false;
            }
    } // close bracket of is_settings == true
}

void Settings_Exit(void) {
    CP_Image_Free(menu_button);
}
