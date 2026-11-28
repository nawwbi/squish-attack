#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "settings.h"
#include "audio.h"
#include "pause.h"
#include "levelintro.h"
#include "settingscontrols.h"
#include <stdbool.h>

CP_Font font;
CP_Image
pause_button,
menu_button;

extern CP_Image background;

extern int is_paused;
bool is_settings = false; 
bool confirm_quit = false;

// controls page 
bool is_controls = false; // initialization to open controls page

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
// extern bool current_attacker; 

void Pause_Menu_Init(void) {
    font = CP_Font_Load("Assets/Exo2-Regular.ttf");
	pause_button = CP_Image_Load("./Assets/pause-button.png");
	menu_button = CP_Image_Load("./Assets/game-button.png");
    Settings_Init(); // Load settings assets here
    Settings_Controls_Init();
}

void Pause_Menu_Update(void) {
    // pause button 
    float pause_button_x = 1500.0f;
    float pause_button_y = 100.0f;
    float pause_button_w = 80.0f;
    float pause_button_h = 80.0f;

    if (!is_paused && !showUpgrade) {
        CP_Image_Draw(pause_button, pause_button_x, pause_button_y, pause_button_w, pause_button_h, 255);

        if (Is_Area_Clicked(pause_button_x, pause_button_y, pause_button_w, pause_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_ESCAPE)) {
            Audio_Play_Button_Click();
            CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC);
            is_paused = 1; // pause state true if pause button clicked
            is_controls = false; // makes sure when opening pause it doesnt open controls auto 
        }
    }

    // pause menu (continue, settings, restart, exit)
    if (is_paused == 1) {

        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Image_Draw(background, 800, 450, 1600, 900, 255);

        if (is_settings == false && confirm_quit == false) {
            Draw_Rectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 400, 650, 20);

            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(50);
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
            CP_Font_DrawText("PAUSED", 800, 195); // pause menu background

            // menu buttons coordinates in pause menu 
            float pmenu_button_w = 320.0f;
            float pmenu_button_h = 90.0f;
            float pmenu_button_x = 800.0f;

            float continue_button_y = 300.0f; // 340 -> 105 diff
            float settings_button_y = 405.0f; // 430
            float pmenu_controls_button_y = 510.0f; // 610
            float restart_button_y = 615.0f; // 520
            float mainmenu_button_y = 720.0f;

            // clicks for menu buttons 
            int continueClicked = Is_Area_Clicked(pmenu_button_x, continue_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int settingsClicked = Is_Area_Clicked(pmenu_button_x, settings_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int pmenuControlsClicked = Is_Area_Clicked(pmenu_button_x, pmenu_controls_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int restartClicked = Is_Area_Clicked(pmenu_button_x, restart_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
            int mainMenuClicked = Is_Area_Clicked(pmenu_button_x, mainmenu_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());

            // menu buttons 
            CP_Image_Draw(menu_button, pmenu_button_x, continue_button_y, pmenu_button_w, 280.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, settings_button_y, pmenu_button_w, 280.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, pmenu_controls_button_y, pmenu_button_w, 280.0f, 255);
            CP_Image_Draw(menu_button, pmenu_button_x, restart_button_y, pmenu_button_w, 280.0f, 255);         
            CP_Image_Draw(menu_button, pmenu_button_x, mainmenu_button_y, pmenu_button_w, 280.0f, 255);

            CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
            CP_Settings_TextSize(30.0f);

            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

            CP_Font_DrawText("Continue", pmenu_button_x, continue_button_y - 10.0f);
            CP_Font_DrawText("Settings", pmenu_button_x, settings_button_y - 10.0f);
            CP_Font_DrawText("Controls", pmenu_button_x, pmenu_controls_button_y - 10.0f);
            CP_Font_DrawText("Restart", pmenu_button_x, restart_button_y - 10.0f);
            CP_Font_DrawText("Main Menu", pmenu_button_x, mainmenu_button_y - 10.0f);

            // continue 
            if (continueClicked == 1) {
                Audio_Play_Button_Click();
                CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
                is_paused = 0;
            }

            // settings 
            else if (settingsClicked == 1) {
               Audio_Play_Button_Click();
               is_settings = true;
            }

            // controls
            else if (pmenuControlsClicked == 1) {
                Audio_Play_Button_Click();
                is_controls = true;
            }

            // restart
            else if (restartClicked == 1) {
                Audio_Play_Button_Click();

                // set everything back to start of game state
                is_paused = 0; // not paused state
                current_round = 1; // restart to round 1
                current_level = 1; // restart to level 1
                showUpgrade = false;
                upgradeTriggered = false;
                gameOver = false;

                fireball_x = 300;
                soundwave_x = 1300;
                player_basic_attack_shot = 0;
                enemy_basic_attack_shot = 1;
                player_turn = true;
                //current_attacker = true;

                Reset_Characters();
                Check_Current_Enemy(current_level);

                // resume music group so next level music can play
                CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);

                // go to level 1 intro
                CP_Engine_SetNextGameState(Level_Intro_Init, Level_Intro_Update, Level_Intro_Exit);
            }

            else if (mainMenuClicked == 1) {
                Audio_Play_Button_Click();
                confirm_quit = true;
            }
        }

        if (is_settings == true) {
            Settings_Update();
            return;
        }

        if (is_controls == true) {
            Settings_Controls_Update();
            return;
        }
        
        if (confirm_quit == true) {
                Draw_Rectangle(CP_Color_Create(0, 0, 0, 220), 800, 450, 600, 300, 0);

                CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
                CP_Settings_TextSize(45);
                CP_Font_DrawText("Are you sure you want to quit?", 800.0f, 370.0f);

                // yes and no buttons
                CP_Image_Draw(menu_button, 680, 500, 250, 260, 255);
                CP_Image_Draw(menu_button, 920, 500, 250, 260, 255);

                CP_Settings_TextSize(40);
                CP_Font_DrawText("Yes", 680.0f, 490.0f);
                CP_Font_DrawText("No", 920.0f, 490.0f);

                int yesClicked = Is_Area_Clicked(680.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

                int noClicked = Is_Area_Clicked(920.0f, 500.0f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY());

                // yes resets game and go back to main menu
                if (yesClicked == 1){
                    Audio_Play_Button_Click();
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
                    enemy_basic_attack_shot = 1;
                    player_turn = true;
                    //current_attacker = true;

                    Reset_Characters();
                    Check_Current_Enemy(current_level);

                    CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
                    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
                }

                // closes popup and stays on pause menu
                if (noClicked == 1){
                    Audio_Play_Button_Click();
                    confirm_quit = false;
                }
         }
     }
}

void Pause_Menu_Exit(void) {
	CP_Image_Free(pause_button);
	CP_Image_Free(menu_button);
    Settings_Exit();
}