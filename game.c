#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "endmenu.h"
#include "losemenu.h"
#include "constants.h"
#include "upgrades.h"
#include "mainmenu.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image 
player_closed, 
player_opened,
dumpling_closed,
dumpling_opened,
butter_closed,
butter_opened,
boss_closed,
boss_opened,
fireball,
soundwave,
current_enemy_closed_img,
current_enemy_opened_img,
skip_button,
pause_button,
menu_button;

CP_Color 
darkRed, 
darkGreen,
grey,
lightGrey,
black,
white;

extern CP_Image background;
float speed = 400.0f;
int speed_multiplier = 1;
int fireball_x = 300;
int soundwave_x = 1300;
int score = 0;
int highscore = 0;

int is_paused = 0; // pause state initialize 
int numSkills;
bool showUpgrade = false;

bool gameOver = false;

//player's attack, defence, current hp, max hp
#define PLAYER_ATTACK                 200.0
#define PLAYER_DEFENCE                50.0
#define PLAYER_CURRENT_HP             1000.0
#define PLAYER_MAX_HP                 1000.0
#define PLAYER_BASIC_ATTACK_ITERATION 2
#define PLAYER_ULTIMATE_ITERATION     1
#define PLAYER_ULTIMATE_CHARGE        2

//dumpling's attack, defence, current hp, max hp
#define DUMPLING_ATTACK                 80.0
#define DUMPLING_DEFENCE                0.0
#define DUMPLING_CURRENT_HP             1000.0
#define DUMPLING_MAX_HP                 1000.0
#define DUMPLING_BASIC_ATTACK_ITERATION 3
#define DUMPLING_ULTIMATE_ITERATION     1
#define DUMPLING_ULTIMATE_CHARGE        2

//butter's attack, defence, current hp, max hp
#define BUTTER_ATTACK                 100.0
#define BUTTER_DEFENCE                50.0
#define BUTTER_CURRENT_HP             1500.0
#define BUTTER_MAX_HP                 1500.0
#define BUTTER_BASIC_ATTACK_ITERATION 2
#define BUTTER_ULTIMATE_ITERATION     1
#define BUTTER_ULTIMATE_CHARGE        2

//boss's attack, defence, current hp, max hp
#define BOSS_ATTACK                 120.0
#define BOSS_DEFENCE                60.0
#define BOSS_CURRENT_HP             1800.0
#define BOSS_MAX_HP                 1800.0
#define BOSS_BASIC_ATTACK_ITERATION 1
#define BOSS_ULTIMATE_ITERATION     1
#define BOSS_ULTIMATE_CHARGE        2

struct Characters player = { PLAYER_ATTACK, PLAYER_DEFENCE, PLAYER_CURRENT_HP, PLAYER_MAX_HP,
    {{ BASIC_ATTACK, PLAYER_BASIC_ATTACK_ITERATION }, { ULTIMATE, PLAYER_ULTIMATE_ITERATION }}
};
struct Characters dumpling = { DUMPLING_ATTACK, DUMPLING_DEFENCE, DUMPLING_CURRENT_HP, DUMPLING_MAX_HP,
    {{ BASIC_ATTACK, DUMPLING_BASIC_ATTACK_ITERATION }, { ULTIMATE, DUMPLING_ULTIMATE_ITERATION }}
};
struct Characters butter = { BUTTER_ATTACK, BUTTER_DEFENCE, BUTTER_CURRENT_HP, BUTTER_MAX_HP,
    {{ BASIC_ATTACK, BUTTER_BASIC_ATTACK_ITERATION }, { ULTIMATE, BUTTER_ULTIMATE_ITERATION }}
};
struct Characters boss = { BOSS_ATTACK, BOSS_DEFENCE, BOSS_CURRENT_HP, BOSS_MAX_HP, 
    {{ BASIC_ATTACK, BOSS_BASIC_ATTACK_ITERATION }, { ULTIMATE, BOSS_ULTIMATE_ITERATION, BOSS_ULTIMATE_CHARGE}}
};
struct Characters* current_enemy;

void Check_Current_Enemy(int current_level) {
    switch (current_level) {
    case 1:
        current_enemy = &dumpling;
        current_enemy_closed_img = dumpling_closed;
        current_enemy_opened_img = dumpling_opened;
        break;
    case 2:
        current_enemy = &butter;
        current_enemy_closed_img = butter_closed;
        current_enemy_opened_img = butter_opened;
        break;
    case 3:
        current_enemy = &boss;
        current_enemy_closed_img = boss_closed;
        current_enemy_opened_img = boss_opened;
        break;
    }
}

void Progress_Bar(int current_level, float dt) {
    CP_Settings_RectMode(CP_POSITION_CENTER);
    DrawRectangle(lightGrey, 800, 100, 999, 25, 10);

    CP_Settings_RectMode(CP_POSITION_CORNER);
    int start_x = 300 + (current_level - 1) * 300;
    int end_x = (current_level == 3) ? 1250 : start_x + 300;

    DrawRectangle(
        darkGreen,
        300, 95,
        (current_level - 1) * 333,
        10, 10
    );

    static float progress_x = 300;

    progress_x += 300 * dt;

    if (progress_x >= end_x || progress_x < start_x) {
        progress_x = start_x;
    }

    DrawRectangle(grey, progress_x, 95, 37, 10, 10);
}

int Game_Per_Round(float dt, int current_round, int current_level) {

    static bool current_attacker = true; // true for player turn, false for enemy turn

    //setting background
    CP_Image_Draw(background, 800, 450, 1600, 900, 255);
    CP_Image_Draw(skip_button, 1500, 800, 100, 100, 255);
    CP_Color black = CP_Color_Create(0, 0, 0, 255);
	CP_Settings_Fill(black);
    char speedMultiplierText[100];
    snprintf(speedMultiplierText, sizeof(speedMultiplierText), "x%d", speed_multiplier);
    CP_Font_DrawText(speedMultiplierText, 1500 , 720);


    if (IsAreaClicked(1500, 800, 100, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		if (speed < 1200.0) {
			speed += 400.0;
            speed_multiplier += 1;
		}
        else if (speed >= 1200.0) {
			speed = 400.0;
            speed_multiplier = 1;
        }
 

    }
    //setting text for rounds
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(40);
    // update the round for each hit when game is not over
    char roundText[50];
    snprintf(roundText, sizeof(roundText), "Round: %d/10", current_round);
    CP_Font_DrawText(roundText, 800, 130);

    char levelText[50];
    snprintf(levelText, sizeof(levelText), "Level: %d/3", current_level);
    CP_Font_DrawText(levelText, 800, 170);

    //--------------------------------------------PLAYER--------------------------------------------\\
    //defence text
    Text(30, white, player.defence, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT - 30, "Defence: %.2f");

    //attack text
    Text(30, white, player.attack, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT, "Attack: %.2f");

    //max hp text
    Text(30, white, player.max_hp, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT + 30, "Max HP: %.2f");
    //--------------------------------------------PLAYER--------------------------------------------\\

    //--------------------------------------------ENEMY--------------------------------------------\\
    //defence text
    Text(30, white, current_enemy->defence, ENEMY_X_AXIS - 20, ENEMY_Y_AXIS - ENEMY_HEIGHT + 30, "Defence: %.2f");

    //attack text
    Text(30, white, current_enemy->attack, ENEMY_X_AXIS - 20, ENEMY_Y_AXIS - ENEMY_HEIGHT + 60, "Attack: %.2f");

    //max hp text
    Text(30, white, current_enemy->max_hp, ENEMY_X_AXIS - 20, ENEMY_Y_AXIS - ENEMY_HEIGHT + 90, "Max HP: %.2f");
    //--------------------------------------------ENEMY--------------------------------------------\\

    //current level status, progress bar
    Progress_Bar(current_level, dt);

    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Image_Draw(player_opened, PLAYER_X_AXIS, 100, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);
    CP_Image_Draw(dumpling_opened, 630, 85, MINI_PLAYER_WIDTH+10, MINI_PLAYER_HEIGHT+10, 255);
    CP_Image_Draw(butter_opened, 950, 80, MINI_PLAYER_WIDTH+15, MINI_PLAYER_HEIGHT+15, 255);
    CP_Image_Draw(boss_opened, 1300, 85, MINI_PLAYER_WIDTH+10, MINI_PLAYER_HEIGHT+10, 255);

    //shadow for player
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(PLAYER_X_AXIS + 25, PLAYER_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
    // display healthbar before rounds
    HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, current_enemy->current_hp, current_enemy->max_hp);
    HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, player.max_hp);

    static int player_basic_attack_shot = 0;
    // if its players turn to attack
    if (current_attacker == true) { 

        if (player_basic_attack_shot < player.skills[BASIC_ATTACK].iteration) {
            //collision check for player fireball and enemy
            bool fireballHitEnemy = AreCirclesIntersecting(fireball_x, PLAYER_Y_AXIS, 90 / 2, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH / 2);

            //if not colliding, fireball continues to travel forward. else, enemy's hp is reduced based off player's attack
            if (!fireballHitEnemy) {
                fireball_x += speed * dt;
                CP_Image_Draw(fireball, fireball_x, PLAYER_Y_AXIS, 90, 90, 255);
                //player open mouth when shooting fireball
                CP_Image_Draw(player_opened, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
                HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(current_enemy->current_hp, 0, current_enemy->defence), CalculateCurrentMaxHP(current_enemy->max_hp, 0));
            }
            else {
                //player close mouth when fireball is shot
                CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
                //base health should be current enemy health, and base attack should be current player attack
                // update current hp with reduced health
                current_enemy->current_hp = CalculateCurrentHP(current_enemy->current_hp, player.attack, current_enemy->defence);
                HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, current_enemy->current_hp, current_enemy->max_hp);
                fireball_x = 300; // reset fireball position after hitting enemy
                
                player_basic_attack_shot++;
                score += (player.attack - current_enemy->defence);
            }
        }
        else {
            player_basic_attack_shot = 0;
            current_attacker = false;
        }
        
    }
    //shadow for enemy
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(ENEMY_X_AXIS + 25, ENEMY_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    //enemy closed mouth
    CP_Image_Draw(current_enemy_closed_img, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);

	static int enemy_basic_attack_shot = 0;
    if (current_attacker == false) {

        //collision check for enemy soundwave and player
        bool soundwaveHitPlayer = AreCirclesIntersecting(soundwave_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, ENEMY_WIDTH / 2);

        if (enemy_basic_attack_shot < current_enemy->skills[BASIC_ATTACK].iteration) {

            //if not colliding, soundwave continues to travel forward. else, player's hp is reduced based off enemy's attack
            if (!soundwaveHitPlayer) {
                soundwave_x -= speed * dt;
                CP_Image_Draw(soundwave, soundwave_x, ENEMY_Y_AXIS, 90, 90, 255);
                // enemy open mouth when shooting soundwave
                CP_Image_Draw(current_enemy_opened_img, ENEMY_X_AXIS, ENEMY_Y_AXIS, ENEMY_WIDTH, ENEMY_HEIGHT, 255);
                HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, player.max_hp);
            }
            else {
                // enemy closed mouth when soundwave is shot
                CP_Image_Draw(current_enemy_closed_img, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
                // update current hp with reduced health
                player.current_hp = CalculateCurrentHP(player.current_hp, current_enemy->attack, player.defence);
                HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, player.max_hp);
                soundwave_x = 1300; // reset soundwave position after hitting player

                score -= (current_enemy->attack - player.defence);
				++enemy_basic_attack_shot;
            }
        }
        else {
			enemy_basic_attack_shot = 0;
			current_attacker = true;

            // resets current_attacker to true for next round which allows player to attack first
            return 1; //allows current round to increment when player is hit by soundwave
        }
    }
    return 0;
}

void Reset_Characters(void)
{
    // player
    player.attack = PLAYER_ATTACK;
    player.defence = PLAYER_DEFENCE;
    player.current_hp = PLAYER_CURRENT_HP;
    player.max_hp = PLAYER_MAX_HP;

    // dumpling
    dumpling.attack = DUMPLING_ATTACK;
    dumpling.defence = DUMPLING_DEFENCE;
    dumpling.current_hp = DUMPLING_CURRENT_HP;
    dumpling.max_hp = DUMPLING_MAX_HP;

    // butter
    butter.attack = BUTTER_ATTACK;
    butter.defence = BUTTER_DEFENCE;
    butter.current_hp = BUTTER_CURRENT_HP;
    butter.max_hp = BUTTER_MAX_HP;

    // boss
    boss.attack = BOSS_ATTACK;
    boss.defence = BOSS_DEFENCE;
    boss.current_hp = BOSS_CURRENT_HP;
    boss.max_hp = BOSS_MAX_HP;
}

void Game_Init(void)
{
    player_closed = CP_Image_Load("./Assets/player-closed.png");
    player_opened = CP_Image_Load("./Assets/player-open.png");
    dumpling_closed = CP_Image_Load("./Assets/dumpling-closed.png");
    dumpling_opened = CP_Image_Load("./Assets/dumpling-open.png");
    butter_closed = CP_Image_Load("./Assets/butter-close.png");
    butter_opened = CP_Image_Load("./Assets/butter-open.png");
    boss_closed = CP_Image_Load("./Assets/boss-closed.png");
    boss_opened = CP_Image_Load("./Assets/boss-open.png");
    fireball = CP_Image_Load("./Assets/fireball.png");
    soundwave = CP_Image_Load("./Assets/soundwave.png");
	skip_button = CP_Image_Load("./Assets/skip-button.png");
    pause_button = CP_Image_Load("./Assets/pause-button.png"); 
    menu_button = CP_Image_Load("./Assets/game_button.png");

    score = 0; // reset score every time new game
    skip_button = CP_Image_Load("./Assets/skip-button.png");

    gameOver = false; // reset game over state for a new game
     
    darkRed = CP_Color_Create(255, 0, 0, 255);
    darkGreen = CP_Color_Create(0, 255, 0, 255);
    grey = CP_Color_Create(70, 70, 70, 225);
    lightGrey = CP_Color_Create(100, 100, 100, 100);
    black = CP_Color_Create(0, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);
}

void Game_Update(void)
{
    //get delta time
    float dt = CP_System_GetDt();
    static int current_round = 1;      // track current round number
    static int current_level = 1;      // track current level number
    //static bool gameOver = false;      // check if game is over (for both win and lose)

    // checking for lose conditions for player and enemy 
    // check if player has killed dumpling , with health over 0 and current round is less than 10

    static bool showUpgrade = false;
    static bool upgradeTriggered = false;

    Check_Current_Enemy(current_level);

    if (current_enemy->current_hp <= 0 &&
        current_round <= 10 &&
        player.current_hp > 0 &&
        !upgradeTriggered && current_level < 4) {

        showUpgrade = true;
        upgradeTriggered = true;
        ++current_level;
    }

    if (current_level > 3) {
        if (highscore < score) {
            highscore = score;
        }
        CP_Engine_SetNextGameState(End_Menu_Init, End_Menu_Update, End_Menu_Exit);
        current_level = 1;
        current_round = 1;
        showUpgrade = false;
        upgradeTriggered = false;
        Reset_Characters();
        Check_Current_Enemy(current_level);
    }

    else if (showUpgrade) {
        CP_Engine_SetNextGameState(Upgrades_Init, Upgrades_Update, Upgrades_Exit);
        current_round = 1;
        showUpgrade = false;
        upgradeTriggered = false;
        return;
    }

    // player loses if they have lesser hp than dumpling
    if (player.current_hp <= 0 && current_enemy->current_hp > 0)
    {
        gameOver = true;

        if (highscore < score)
        {
            highscore = score;
        }

        CP_Engine_SetNextGameState(Lose_Menu_Init, Lose_Menu_Update, Lose_Menu_Exit);

        current_level = 1;
        current_round = 1;
        showUpgrade = false;
        upgradeTriggered = false;

        Reset_Characters();
        Check_Current_Enemy(current_level);

        return;
    }
    // if current round exceeds 10, game is over
    if (current_round > 10)
    {
        gameOver = true;

        if (highscore < score)
        {
            highscore = score;
        }

        CP_Engine_SetNextGameState(Lose_Menu_Init, Lose_Menu_Update, Lose_Menu_Exit);

        current_level = 1;
        current_round = 1;
        showUpgrade = false;
        upgradeTriggered = false;

        Reset_Characters();
        Check_Current_Enemy(current_level);

        return;
    }
    // update the round for each hit when game is not over
    if (!gameOver && current_round <= 10) {
        if (Game_Per_Round(dt, current_round, current_level) == 1) {
            ++current_round;
        }
    }

    // pause button 
    float pause_button_x = 1500.0f;
    float pause_button_y = 100.0f;
    float pause_button_w = 80.0f;
    float pause_button_h = 80.0f;

    if (!is_paused && !showUpgrade) {
        CP_Image_Draw(pause_button, pause_button_x, pause_button_y, pause_button_w, pause_button_h, 255);

        if (IsAreaClicked(pause_button_x, pause_button_y, pause_button_w, pause_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
            is_paused = 1; // pause state true if pause button clicked
        }
    }

    /*if (is_paused) {
        dt = 0.0f; // freeze time in pause state
    }*/

    // pause menu (continue, settings, restart, exit)
    if (is_paused) {
        CP_Settings_RectMode(CP_POSITION_CENTER);
        DrawRectangle(CP_Color_Create(90, 190, 245, 180), 800, 450, 400, 500, 20);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(50);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("PAUSED", 800, 260); // pause menu background

        // menu buttons coordinates in pause menu 
        float pmenu_button_w = 300.0f;
        float pmenu_button_h = 260.0f;
        float pmenu_button_x = 800.0f;

        float continue_button_y = 340.0f; // 340
        float settings_button_y = 430.0f; // 430
        float restart_button_y =  520.0f; // 520 
        float mainmenu_button_y = 610.0f; // 610

        // clicks for menu buttons 
        bool continueClicked = IsAreaClicked(pmenu_button_x, continue_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
        bool settingsClicked = IsAreaClicked(pmenu_button_x, settings_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
        bool restartClicked = IsAreaClicked(pmenu_button_x, restart_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());
        bool mainMenuClicked = IsAreaClicked(pmenu_button_x, mainmenu_button_y, pmenu_button_w, pmenu_button_h, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        // menu buttons 
        CP_Image_Draw(menu_button, pmenu_button_x, continue_button_y, pmenu_button_w, pmenu_button_h, 255);
        CP_Image_Draw(menu_button, pmenu_button_x, settings_button_y, pmenu_button_w, pmenu_button_h, 255);
        CP_Image_Draw(menu_button, pmenu_button_x, restart_button_y, pmenu_button_w, pmenu_button_h, 255);
        CP_Image_Draw(menu_button, pmenu_button_x, mainmenu_button_y, pmenu_button_w, pmenu_button_h, 255);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(30.0f);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

        CP_Font_DrawText("Continue", pmenu_button_x, continue_button_y);
        CP_Font_DrawText("Settings", pmenu_button_x, settings_button_y);
        CP_Font_DrawText("Restart", pmenu_button_x, restart_button_y);
        CP_Font_DrawText("Main Menu", pmenu_button_x, mainmenu_button_y);

        // continue 
        if (continueClicked) {
            is_paused = 0; 
        }
        // settings 
        else if (settingsClicked) {
            // todo
        } 
        // restart
        else if (restartClicked) {
            is_paused = 0;
            current_round = 1;
            current_level = 1;
            showUpgrade = false; 
            upgradeTriggered = false;
            gameOver = false;

            Reset_Characters();
            Check_Current_Enemy(current_level);
   
        }
        else if (mainMenuClicked) {
            is_paused = 0;
            CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
        }
    }
}

void Game_Exit(void)
{
    CP_Image_Free(player_opened);
    CP_Image_Free(player_closed);
    CP_Image_Free(dumpling_closed);
    CP_Image_Free(dumpling_opened);
    CP_Image_Free(butter_closed);
    CP_Image_Free(butter_opened);
    CP_Image_Free(boss_closed);
    CP_Image_Free(boss_opened);
}