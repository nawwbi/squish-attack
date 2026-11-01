#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "constants.h"
#include "upgrades.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image player_closed = NULL;
CP_Image player_opened = NULL;
CP_Image dumpling_closed = NULL;
CP_Image dumpling_opened = NULL;
CP_Image boss_closed = NULL;
CP_Image boss_opened = NULL;
CP_Image fireball = NULL;
CP_Image soundwave = NULL;
extern CP_Image background;
float speed = 800.0f;
int fireball_x = 300;
int soundwave_x = 1300;

// attack , defence , current_hp , max_hp
struct Characters player = { 150.0, 50.0, 1000.0, 1000.0 };
// enemy values to be updated based on user testing
struct Characters dumpling = { 100.0, 50.0, 1000.0, 1000.0 };
struct Characters butter = { 80.0, 50.0, 1500.0, 1500.0 };
struct Characters boss = { 120.0, 60.0, 1800.0, 1800.0 };

int Game_Per_Round(float dt, CP_Color darkRed, CP_Color darkGreen, int current_round) {

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

    //setting text for player defence
    CP_Settings_TextSize(30);
    char defenceText[50];
    snprintf(defenceText, sizeof(defenceText), "Defence: %.2f", CalculateCurrentDefence(player.defence, 0));
    CP_Font_DrawText(defenceText,
        PLAYER_X_AXIS - 20,
        PLAYER_Y_AXIS - PLAYER_HEIGHT - 15);

    //setting text for player attack
    char attackText[50];
    snprintf(attackText, sizeof(attackText), "Attack: %.2f", CalculateCurrentAttack(player.attack, 0));
    CP_Font_DrawText(attackText,
        PLAYER_X_AXIS - 20,
        PLAYER_Y_AXIS - PLAYER_HEIGHT + 15);

    //setting text for dumpling defence
    CP_Settings_TextSize(30);
    snprintf(defenceText, sizeof(defenceText), "Defence: %.2f", CalculateCurrentDefence(dumpling.defence, 0));
    CP_Font_DrawText(defenceText,
        ENEMY_X_AXIS - 20,
        ENEMY_Y_AXIS - ENEMY_HEIGHT + 60);

    //setting text for dumpling attack
    snprintf(attackText, sizeof(attackText), "Attack: %.2f", CalculateCurrentAttack(dumpling.attack, 0));
    CP_Font_DrawText(attackText,
        ENEMY_X_AXIS - 20,
        ENEMY_Y_AXIS - ENEMY_HEIGHT + 90);

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
    HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, 0, dumpling.defence), CalculateCurrentMaxHP(dumpling.max_hp, 0));
    HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, CalculateCurrentHP(player.current_hp, 0, player.defence), CalculateCurrentMaxHP(player.max_hp, 0));

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
            HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, CalculateCurrentHP(dumpling.current_hp, 0, dumpling.defence), CalculateCurrentMaxHP(dumpling.max_hp, 0));
        }
        else {
            //player close mouth when fireball is shot
            CP_Image_Draw(player_closed, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
            //base health should be current enemy health, and base attack should be current player attack
            // update current hp with reduced health
            dumpling.current_hp = CalculateCurrentHP(dumpling.current_hp, player.attack, dumpling.defence);
            HealthBar(darkRed, 1250, ENEMY_Y_AXIS - PLAYER_HEIGHT + 120, dumpling.current_hp, CalculateCurrentMaxHP(dumpling.max_hp, 0));
            fireball_x = 300; // reset fireball position after hitting enemy
            current_attacker = false;
        }
    }
    //shadow for enemy
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
    CP_Graphics_DrawEllipse(ENEMY_X_AXIS + 25, ENEMY_Y_AXIS + 90, PLAYER_WIDTH, 20.0f);

    //enemy closed mouth
    CP_Image_Draw(dumpling_closed, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);

    if (current_attacker == false) {
		//collision check for enemy soundwave and player
        bool soundwaveHitPlayer = AreCirclesIntersecting(soundwave_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, ENEMY_WIDTH / 2);

		//if not colliding, soundwave continues to travel forward. else, player's hp is reduced based off enemy's attack
        if (!soundwaveHitPlayer) {
            soundwave_x -= speed * dt;
            CP_Image_Draw(soundwave, soundwave_x, ENEMY_Y_AXIS, 90, 90, 255);
            // enemy open mouth when shooting soundwave
            CP_Image_Draw(dumpling_opened, ENEMY_X_AXIS, ENEMY_Y_AXIS, ENEMY_WIDTH, ENEMY_HEIGHT, 255);
            HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, CalculateCurrentHP(player.current_hp, 0, player.defence), CalculateCurrentMaxHP(player.max_hp, 0));
        }
        else {
			// enemy closed mouth when soundwave is shot
            CP_Image_Draw(dumpling_closed, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH, PLAYER_HEIGHT, 255);
            // update current hp with reduced health
            player.current_hp = CalculateCurrentHP(player.current_hp, dumpling.attack, player.defence);
            HealthBar(darkGreen, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, CalculateCurrentMaxHP(player.max_hp, 0));
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
    boss_closed = CP_Image_Load("./Assets/boss-closed.png");
    boss_opened = CP_Image_Load("./Assets/boss-open.png");
    fireball = CP_Image_Load("./Assets/fireball.png");
    soundwave = CP_Image_Load("./Assets/soundwave.png");
}

void Game_Update(void)
{
    CP_Color darkRed = CP_Color_Create(255, 0, 0, 255);
    CP_Color darkGreen = CP_Color_Create(0, 255, 0, 255);
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    //get delta time
    float dt = CP_System_GetDt();
    static int current_round = 1;      // track current round number
    static bool gameOver = false;      // check if game is over (for both win and lose)

	// checking for lose conditions for player and enemy 
	// check if player has killed dumpling , with health over 0 and current round is less than 10
    if (dumpling.current_hp <= 0 && current_round <= 10 && player.current_hp > 0) {
        gameOver = true;

        CP_Settings_Tint(CP_Color_Create(0, 0, 0, 200));
        soundwave = 1300;
        CP_Engine_SetNextGameState(Upgrades_Init, Upgrades_Update, Upgrades_Exit); // displays upgrade screen after player wins a round and is still alive
        return;
    }

    // player loses if they have lesser hp than dumpling
    if (player.current_hp <= 0 && dumpling.current_hp > 0) {
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
        if (Game_Per_Round(dt, darkRed, darkGreen, current_round) == 1) {
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
}