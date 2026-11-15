#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "pause.h"
#include <stdbool.h>

CP_Image
pause_button,
menu_button;

extern int is_paused;
int is_settings = 0; 
int confirm_quit = 0;

extern int fireball_x;
extern int soundwave_x;
extern int current_round;
extern int current_level;
extern bool showUpgrade;
extern bool upgradeTriggered;
extern bool gameOver; 
extern int player_basic_attack_shot;
extern int enemy_basic_attack_shot;
extern bool player_turn;
extern bool current_attacker; 

void Pause_Menu_Init(void) {
	pause_button = CP_Image_Load("./Assets/pause-button.png");
	menu_button = CP_Image_Load("./Assets/game_button.png");
}

void Pause_Menu_Update(void) {
    // pause button 
    float pause_button_x = 1500.0f;
    float pause_button_y = 100.0f;
    float pause_button_w = 80.0f;
    float pause_button_h = 80.0f;

    if (!is_paused && !showUpgrade) {
        CP_Image_Draw(pause_button, pause_button_x, pause_button_y, pause_button_w, pause_button_h, 255);

        if (IsAreaClicked(pause_button_x, pause_button_y, pause_button_w, pause_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
            is_paused = 1; // pause state true if pause button clicked
        }
    }

    /*if (is_paused) {
        dt = 0.0f; // freeze time in pause state
    }*/

    // pause menu (continue, settings, restart, exit)
    if (is_paused == 1) {
        CP_Settings_RectMode(CP_POSITION_CENTER);

        if (!is_settings && !confirm_quit) {
            DrawRectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 400, 500, 20);

            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("PAUSED", 800, 245); // pause menu background

            // menu buttons coordinates in pause menu 
            float pmenu_button_w = 320.0f;
            float pmenu_button_h = 90.0f;
            float pmenu_button_x = 800.0f;

            float continue_button_y = 330.0f; // 340
            float settings_button_y = 435.0f; // 430
            float restart_button_y = 540.0f; // 520 
            float mainmenu_button_y = 645.0f; // 610

            // clicks for menu buttons 
            int continueClicked = IsAreaClicked(pmenu_button_x, continue_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int settingsClicked = IsAreaClicked(pmenu_button_x, settings_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int restartClicked = IsAreaClicked(pmenu_button_x, restart_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int mainMenuClicked = IsAreaClicked(pmenu_button_x, mainmenu_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());

            // menu buttons 
            CP_Image_Draw(menu_button, pmenu_button_x, continue_button_y, pmenu_button_w, 220.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, settings_button_y, pmenu_button_w, 220.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, restart_button_y, pmenu_button_w, 220.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, mainmenu_button_y, pmenu_button_w, 220.0f, 255);

            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(30.0f);

            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

            CP_Font_DrawText("Continue", pmenu_button_x, continue_button_y - 10.0f);
            CP_Font_DrawText("Settings", pmenu_button_x, settings_button_y - 10.0f);
            CP_Font_DrawText("Restart", pmenu_button_x, restart_button_y - 10.0f);
            CP_Font_DrawText("Main Menu", pmenu_button_x, mainmenu_button_y - 10.0f);

            // continue 
            if (continueClicked == 1) {
                is_paused = 0;
            }
            // settings 
            else if (settingsClicked == 1) {
                is_settings = 1;
            }
            // restart
            else if (restartClicked == 1) {
                is_paused = 0; // not paused state
                current_round = 1; // restart to round 1
                current_level = 1; // restart to level 1
                showUpgrade = false;
                upgradeTriggered = false;
                gameOver = false;

                fireball_x = 300;
                soundwave_x = 1300;
                player_basic_attack_shot = 0;
                enemy_basic_attack_shot = 0;
                current_attacker = true;

                Reset_Characters();
                Check_Current_Enemy(current_level);

            }
            else if (mainMenuClicked == 1) {
                confirm_quit = 1;
            }
        }

        if (is_settings == 1) {
            CP_Settings_RectMode(CP_POSITION_CENTER);
            DrawRectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 600, 700, 20);

            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("SETTINGS", 800, 150);

            // back button in settings 
            float settings_back_w = 250.0f;
            float settings_back_h = 80.0f;
            float settings_back_x = 800.0f;
            float settings_back_y = 650.0f;

            CP_Image_Draw(menu_button, settings_back_x, settings_back_y, settings_back_w, settings_back_h, 255);
            CP_Settings_TextSize(25.0f);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("Back", settings_back_x, settings_back_y - 10.0f);

            if (IsAreaClicked(settings_back_x, settings_back_y, settings_back_w, settings_back_h, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                is_settings = 0;
                return;
            }
        }
        
        if (confirm_quit == 1) {
                DrawRectangle(CP_Color_Create(0, 0, 0, 220), 800, 450, 600, 300, 0);

                CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
                CP_Settings_TextSize(45);
                CP_Font_DrawText("Are you sure you want to quit?", 800.0f, 370.0f);

                // yes and no buttons
                CP_Image_Draw(menu_button, 680, 500, 250, 260, 255);
                CP_Image_Draw(menu_button, 920, 500, 250, 260, 255);

                CP_Settings_TextSize(40);
                CP_Font_DrawText("Yes", 680.0f, 490.0f);
                CP_Font_DrawText("No", 920.0f, 490.0f);

                int yesClicked = IsAreaClicked(680.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

                int noClicked = IsAreaClicked(920.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

                // yes resets game and go back to main menu
                if (yesClicked == 1){
                    confirm_quit = 0;
                    is_paused = 0;

                    // restart game
                    current_round = 1;
                    current_level = 1;
                    showUpgrade = false;
                    upgradeTriggered = false;
                    gameOver = false;

                    fireball_x = 300;
                    soundwave_x = 1300;
                    player_basic_attack_shot = 0;
                    enemy_basic_attack_shot = 0;
                    current_attacker = true;
                    player_turn = true;

                    Reset_Characters();
                    Check_Current_Enemy(current_level);
                    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
                }

                // closes popup and stays on pause menu
                if (noClicked == 1){
                    confirm_quit = 0;
                    return;
                }
         }
     }
}

void Pause_Menu_Exit(void) {
	CP_Image_Free(pause_button);
	CP_Image_Free(menu_button);
}