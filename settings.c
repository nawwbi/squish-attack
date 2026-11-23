#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "pause.h"
#include "audio.h"
#include "settings.h"
#include "settingscontrols.h"
#include "debug.h"
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

// controls page 
bool is_controls = false; // initialization to open controls page

// debug mode button in settings 
float debug_mode_x = 800.0f;
float debug_mode_y = 675.0f;
float debug_mode_w = 350.0f;
float debug_mode_h = 220.0f;

void Update_Audio(void) {
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, bgm_on ? main_volume : 0.0f);
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, sfx_on ? main_volume : 0.0f);
}
void Settings_Init(void) {
    CP_Settings_RectMode(CP_POSITION_CENTER);
	menu_button = CP_Image_Load("./Assets/game_button.png");
    white = CP_Color_Create(255, 255, 255, 255);
    green = CP_Color_Create(100, 255, 100, 255);

    Update_Audio();
    Settings_Controls_Init(); // load controls 
}

void Settings_Update(void) {
    if (is_settings == true) {

        DrawRectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 600, 700, 20);

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

        DrawRectangle(green, 610, 320, 50, 40, 0);
        if (IsAreaClicked(610, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
        {
            Audio_Play_Button_Click();
            main_volume = 0.0f;
            Update_Audio();
        }

        // volume 25%
        if (IsAreaClicked(700, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
        {
            Audio_Play_Button_Click();
            main_volume = 0.25f;
            Update_Audio();
        }

        // volume 50%
        if (IsAreaClicked(790, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
        {
            Audio_Play_Button_Click();
            main_volume = 0.50f;
            Update_Audio();
        }

        // volume 75%
        if (IsAreaClicked(880, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
        {
            Audio_Play_Button_Click();
            main_volume = 0.75f;
            Update_Audio();
        }

        // volume 100%
        CP_Settings_Fill(white);
        CP_Settings_TextSize(50);
        CP_Font_DrawText("100", 1040, 320);

        if (IsAreaClicked(970, 320, 50, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
        {
            Audio_Play_Button_Click();
            main_volume = 1.0f;
            Update_Audio();
        }

        // green volume boxes
        // green box 25%
        if (main_volume >= 0.25f) {
            DrawRectangle(green, 700, 320, 50, 40, 0);
        }
        else {
            DrawRectangle(white, 700, 320, 50, 40, 0);
        }

        // green box 50%
        if (main_volume >= 0.50f) {
            DrawRectangle(green, 790, 320, 50, 40, 0);
        }
        else {
            DrawRectangle(white, 790, 320, 50, 40, 0);
        }

        // green box 75%
        if (main_volume >= 0.75f) {
            DrawRectangle(green, 880, 320, 50, 40, 0);
        }
        else {
            DrawRectangle(white, 880, 320, 50, 40, 0);
        }

        // green box 100%
        if (main_volume >= 1.0f) {
            DrawRectangle(green, 970, 320, 50, 40, 0);
        }
        else {
            DrawRectangle(white, 970, 320, 50, 40, 0);
        }

        CP_Settings_Fill(white); // reset text fill color back to white after drawing green volume boxes

        // BGM and SFX settings text
        CP_Settings_Fill(white);
        CP_Settings_TextSize(50);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("BGM & SFX", 800, 420);

        // background music on / off
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(menu_button, 660, 510, 350, 220, 255);
        CP_Settings_TextSize(25.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText(bgm_on ? "Background Music: On" : "Background Music: Off", 660, 500);

        if (IsAreaClicked(660, 510, 350, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            bgm_on = !bgm_on;
            Update_Audio();
            Audio_Play_Button_Click();
        }

        // sound effect on / off
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(menu_button, 940, 510, 350, 220, 255);
        CP_Settings_TextSize(25.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText(sfx_on ? "Sound Effects: On" : "Sound Effects: Off", 940, 500);

        if (IsAreaClicked(940, 510, 350, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            sfx_on = !sfx_on;
            Update_Audio();
            Audio_Play_Button_Click();
        }

        // controls button
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(menu_button, 800, 600, 350, 220, 255);
        CP_Settings_TextSize(25.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("Controls", 800, 600 - 10.0f);

        if (IsAreaClicked(800, 600, 350, 220, CP_Input_GetMouseX(), CP_Input_GetMouseY())) 
        {
            Audio_Play_Button_Click();
            is_controls = true;
        }

        if (is_controls == true)
        {
            Settings_Controls_Update();
            return;
        }

        // debug mode button 
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(menu_button, debug_mode_x, debug_mode_y, debug_mode_w, debug_mode_h, 255);
        CP_Settings_TextSize(25.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("Debug Mode", debug_mode_x, debug_mode_y - 10.0f);

        if (IsAreaClicked(debug_mode_x, debug_mode_y, debug_mode_w, debug_mode_h, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            Audio_Play_Button_Click();
        }

        // back button
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(menu_button, 800, 750, 350, 220, 255);
        CP_Settings_TextSize(25.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("Back", 800, 740);

        if (IsAreaClicked(800, 750, 350, 220, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            Audio_Play_Button_Click();
            is_settings = false;
        }
    }
}

void Settings_Exit(void) {
    CP_Image_Free(menu_button);
}
