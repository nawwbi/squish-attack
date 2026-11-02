#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "endmenu.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image player_closed = NULL;
CP_Image player_opened = NULL;

CP_Image dumpling_closed = NULL;
CP_Image dumpling_opened = NULL;

CP_Image butter_closed = NULL;
CP_Image butter_opened = NULL;

CP_Image boss_closed = NULL;
CP_Image boss_opened = NULL;

CP_Image fireball = NULL;
CP_Image soundwave = NULL;
extern CP_Image background;
float speed = 800.0f;
int fireball_x = 300;
int soundwave_x = 1300;

int clicked = 0;

// attack , defence , current_hp , max_hp
Characters player = { 1000.0, 50.0, 1000.0, 1000.0 };
// enemy values to be updated based on user testing
Characters dumpling = { 100.0, 0.0, 1000.0, 1000.0 };
Characters butter = { 80.0, 50.0, 1500.0, 1500.0 };
Characters boss = { 120.0, 60.0, 1800.0, 1800.0 };

CP_Image button;

CP_Image defence_icon;
CP_Image hp_icon;
CP_Image attack_icon;

int numSkills;
bool showUpgrade = false;

Characters* current_enemy;

CP_Image current_enemy_closed_img, current_enemy_opened_img;

struct Upgrades upgrade[] = {
    { DEFENCE, "+5% Defence", 360, 220, 360 },
    { ATTACK, "+10% Attack", 810, 670, 810 },
    { HP, "+20% Max HP", 1260, 1120, 1260 }
};

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

int Game_Per_Round(float dt, CP_Color darkRed, CP_Color darkGreen, int current_round, int current_level) {

    static bool current_attacker = true; // true for player turn, false for enemy turn

    //setting background
    CP_Image_Draw(background, 800, 450, 1600, 900, 255);

    //setting text for rounds
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_TextSize(40);
    // update the round for each hit when game is not over
    char roundText[50];
    snprintf(roundText, sizeof(roundText), "Round: %d/10", current_round);
    CP_Font_DrawText(roundText, 800, 130);

    char levelText[50];
    snprintf(levelText, sizeof(levelText), "Round: %d/3", current_level);
    CP_Font_DrawText(levelText, 800, 170);

    CP_Color white = CP_Color_Create(255, 255, 255, 255);

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

    //current level status, bar at the top
    CP_Settings_RectMode(CP_POSITION_CENTER);
    DrawRectangle(CP_Color_Create(100, 100, 100, 100), 800, 100, 1000, 10, 10);
    CP_Image_Draw(player_opened, PLAYER_X_AXIS, 100, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);
    CP_Image_Draw(dumpling_opened, 600, 90, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);

    //shadow for player
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(PLAYER_X_AXIS + 25, PLAYER_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
    // display healthbar before rounds
    HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, current_enemy->current_hp, current_enemy->max_hp);
    HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, player.max_hp);

    // if its players turn to attack
    if (current_attacker == true) { 

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
            current_attacker = false;
        }
    }
    //shadow for enemy
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(ENEMY_X_AXIS + 25, ENEMY_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    //enemy closed mouth
    CP_Image_Draw(current_enemy_closed_img, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);

    if (current_attacker == false) {
		//collision check for enemy soundwave and player
        bool soundwaveHitPlayer = AreCirclesIntersecting(soundwave_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, ENEMY_WIDTH / 2);

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

			// resets current_attacker to true for next round which allows player to attack first
            current_attacker = true;
            if (soundwaveHitPlayer) {
				return 1; //allows current round to increment when player is hit by soundwave
            }
        }
    }
    return 0;
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

    button = CP_Image_Load("./Assets/defence_button.png");
    defence_icon = CP_Image_Load("./Assets/defence-icon.png");
    hp_icon = CP_Image_Load("./Assets/hp-icon.png");
    attack_icon = CP_Image_Load("./Assets/fireball.png");

    upgrade[0].image = button;
    upgrade[1].image = button;
    upgrade[2].image = button;

    upgrade[0].icon = defence_icon;
    upgrade[1].icon = attack_icon;
    upgrade[2].icon = hp_icon;

    numSkills = sizeof(upgrade) / sizeof(upgrade[0]);
}

void Game_Update(void)
{
    

    CP_Color darkRed = CP_Color_Create(255, 0, 0, 255);
    CP_Color darkGreen = CP_Color_Create(0, 255, 0, 255);
    //get delta time
    float dt = CP_System_GetDt();
    static int current_round = 1;      // track current round number
    static int current_level = 1;      // track current level number
    static bool gameOver = false;      // check if game is over (for both win and lose)
    static int upgradeEnum = 0;

    // checking for lose conditions for player and enemy 
    // check if player has killed dumpling , with health over 0 and current round is less than 10

    // Check whether the upgrade screen should appear
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
        CP_Engine_SetNextGameState(End_Menu_Init, End_Menu_Update, End_Menu_Exit);
        current_level = 1;
        current_round = 1;
        showUpgrade = false;
    }
    // Display upgrade screen
    else if (showUpgrade) {

        CP_Color black = CP_Color_Create(0, 0, 0, 255);

        for (int i = 0; i < numSkills ; ++i) {
            CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
            CP_Settings_RectMode(CP_POSITION_CENTER);

            clicked = IsAreaClicked(
                upgrade[i].image_x,
                BUTTON_Y_AXIS,
                300,
                500,
                CP_Input_GetMouseX(),
                CP_Input_GetMouseY()
            );

            if (clicked) {
                upgradeEnum = upgrade[i].skill;
                break;
            }

            SkillUpgradeButton(
                upgrade[i].skill,
                upgrade[i].image,
                upgrade[i].image_x,
                BUTTON_Y_AXIS,
                BUTTON_WIDTH,
                BUTTON_HEIGHT,
                black,
                upgrade[i].name,
                upgrade[i].text_x,
                520,
                upgrade[i].icon,
                upgrade[i].icon_x,
                ICON_Y_AXIS,
                ICON_WIDTH,
                ICON_HEIGHT
            );
        }

        // Apply selected upgrade
        if (clicked && upgradeEnum >= 0) {

            switch (upgradeEnum) {

                case DEFENCE:
                    player.defence = CalculateCurrentDefence(player.defence, 0.05);
                    break;

                case ATTACK:
                    player.attack = CalculateCurrentAttack(player.attack, 0.1);
                    break;

                case HP:
                    player.max_hp = CalculateCurrentMaxHP(player.max_hp, 0.2);
                    break;

                case HEAL:
                    player.current_hp = CalculateHealing(player.current_hp, player.max_hp, 0.2);
                    break;
            }

            upgradeEnum = 0;
            // Restore enemy and player HP
            //dumpling.current_hp = dumpling.max_hp;

            // Reset upgrade selection
            current_round = 0;
            showUpgrade = false;
            upgradeTriggered = false;
            
            clicked = 0;

            return;
        }

        return; // Prevent normal gameplay while selecting upgrades
    }

    // player loses if they have lesser hp than dumpling
    if (player.current_hp <= 0 && current_enemy->current_hp > 0) {
        gameOver = true;
        CP_Font_DrawText("GAME OVER (died before dumpling)", 500, 500); // temporary test
        return;
    }
    // if current round exceeds 10, game is over
    if (current_round > 10) {
        gameOver = true;
        CP_Font_DrawText("GAME OVER (Went over 10 rounds)", 500, 500); // temporary test
        return;
    }
    // update the round for each hit when game is not over
    if (!gameOver && current_round <= 10) {
        if (Game_Per_Round(dt, darkRed, darkGreen, current_round, current_level) == 1) {
            ++current_round;
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

    CP_Image_Free(button);
    CP_Image_Free(defence_icon);
    CP_Image_Free(hp_icon);
    CP_Image_Free(attack_icon);
}