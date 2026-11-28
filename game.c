#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "endmenu.h"
#include "constants.h"
#include "upgrades.h"
#include "mainmenu.h"
#include "pause.h"
#include "audio.h"
#include "debug.h"
#include "animation.h"
#include <stdio.h>
#include <stdbool.h>

CP_Image player_sprite = NULL;
CP_Image dumpling_sprite = NULL;
CP_Image butter_sprite = NULL;
CP_Image boss_sprite = NULL;
CP_Image current_enemy_sprite = NULL;
CP_Image spaceship_sprite = NULL;
CP_Image spaceship_icon = NULL;
CP_Image meteorite_sprite = NULL;
CP_Image meteorite_icon = NULL;
CP_Image bubble_sprite = NULL;
CP_Image bubble_icon = NULL;
CP_Image skill_button = NULL;
CP_Image skill_button_grey = NULL;

CP_Image
player_opened,
dumpling_opened,
butter_opened,
boss_opened,
fireball,
soundwave,
skip_button,
defence_icon,
hp_icon;

CP_Color 
dark_red, 
dark_green,
grey,
light_grey,
black,
white;

extern CP_Image background;
float speed = 400.0f;
int speed_multiplier = 1;
float fireball_x = 300;
float soundwave_x = 1300;
float enemy_x = 1300;
int score = 0;
int highscore = 0;

// global variable for game reset or restart 
bool show_upgrade = false;
int player_basic_attack_shot = 0; 
int enemy_basic_attack_shot = 0; 
bool player_turn = true;
int current_round = 1;
int current_level = 1;
int mana = 0;
bool upgrade_triggered = false;
int is_paused = 0; // pause state initialize

bool game_over = false;

float game_fade = 255.0f;

static float current_x = 1300.0f;

float enemy_damage_text_timer = 0.0f;
float enemy_damage_text_value = 0.0f;
float player_damage_text_timer = 0.0f;
float player_damage_text_value = 0.0f;
float stunned_text_timer = 0.0f;
#define DAMAGE_TEXT_DURATION 1.0f

//player's attack, defence, current hp, max hp
#define PLAYER_ATTACK                 350.0
#define PLAYER_DEFENCE                50.0
#define PLAYER_CURRENT_HP             3000.0
#define PLAYER_MAX_HP                 3000.0
#define PLAYER_BASIC_ATTACK_ITERATION 2

#define PLAYER_ULTIMATE_ITERATION     1
#define PLAYER_ULTIMATE_CHARGE        2

#define PLAYER_SPACESHIP_ITERATION    1
#define PLAYER_SPACESHIP_CHARGE       35

#define PLAYER_METEORITE_ITERATION    1
#define PLAYER_METEORITE_CHARGE       40

#define PLAYER_BUBBLE_ITERATION       1
#define PLAYER_BUBBLE_CHARGE          15

//dumpling's attack, defence, current hp, max hp
#define DUMPLING_ATTACK                 80.0
#define DUMPLING_DEFENCE                0.0
#define DUMPLING_CURRENT_HP             4000.0
#define DUMPLING_MAX_HP                 4000.0
#define DUMPLING_BASIC_ATTACK_ITERATION 3
#define DUMPLING_ULTIMATE_ITERATION     1
#define DUMPLING_ULTIMATE_CHARGE        2

//butter's attack, defence, current hp, max hp
#define BUTTER_ATTACK                 100.0
#define BUTTER_DEFENCE                30.0
#define BUTTER_CURRENT_HP             5000.0
#define BUTTER_MAX_HP                 5000.0
#define BUTTER_BASIC_ATTACK_ITERATION 2
#define BUTTER_ULTIMATE_ITERATION     1
#define BUTTER_ULTIMATE_CHARGE        2

//boss's attack, defence, current hp, max hp
#define BOSS_ATTACK                 120.0
#define BOSS_DEFENCE                60.0
#define BOSS_CURRENT_HP             6000.0
#define BOSS_MAX_HP                 6000.0
#define BOSS_BASIC_ATTACK_ITERATION 1
#define BOSS_ULTIMATE_ITERATION     1
#define BOSS_ULTIMATE_CHARGE        3

struct Characters player = { PLAYER_ATTACK, PLAYER_DEFENCE, PLAYER_CURRENT_HP, PLAYER_MAX_HP,
    {
        { BASIC_ATTACK, PLAYER_BASIC_ATTACK_ITERATION }, 
        { ULTIMATE, PLAYER_ULTIMATE_ITERATION },
        { SPACESHIP, PLAYER_SPACESHIP_ITERATION, PLAYER_SPACESHIP_CHARGE},
        { METEORITE, PLAYER_METEORITE_ITERATION, PLAYER_METEORITE_CHARGE}
    }
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

static enum PlayerState {
    PLAYER_IDLE,
    PLAYER_ATTACKING
} PlayerState = PLAYER_IDLE;

static enum EnemyState {
    ENEMY_IDLE,
    ENEMY_ATTACKING
} EnemyState = ENEMY_IDLE;

void Check_Current_Player_State(int player_state) {
    switch (player_state) {
    case PLAYER_IDLE:
        Character_Animation(player_sprite, true, true, PLAYER_X_AXIS);
        break;

    case PLAYER_ATTACKING:
        Character_Animation(player_sprite, true, false, PLAYER_X_AXIS);
        break;

    default:
        Character_Animation(player_sprite, true, true, PLAYER_X_AXIS);
        break;
    }
}

void Check_Current_Enemy_State(int enemy_state) {
    switch (enemy_state) {
    case ENEMY_IDLE:
        Character_Animation(current_enemy_sprite, false, true, enemy_x);
        break;

    case ENEMY_ATTACKING:
        Character_Animation(current_enemy_sprite, false, false, enemy_x);
        break;

    default:
        Character_Animation(current_enemy_sprite, false, true, enemy_x);
        break;
    }
}

void Check_Current_Enemy(int current_level) {
    switch (current_level) {
    case 1:
        current_enemy = &dumpling;
        current_enemy_sprite = dumpling_sprite;
        break;
    case 2:
        current_enemy = &butter;
        current_enemy_sprite = butter_sprite;
        break;
    case 3:
        current_enemy = &boss;
        current_enemy_sprite = boss_sprite;
        break;
    }
}

void Progress_Bar(int current_level, float dt) {
    CP_Settings_RectMode(CP_POSITION_CENTER);
    Draw_Rectangle(light_grey, 800, 100, 999, 25, 10);

    CP_Settings_RectMode(CP_POSITION_CORNER);
    float start_x = 300 + (current_level - 1) * 300.0f;
    float end_x = (current_level == 3) ? 1250 : start_x + 300;

    Draw_Rectangle(
        dark_green,
        300, 95,
        (current_level - 1) * 333.0f,
        10, 10
    );

    static float progress_x = 300;

    progress_x += 300 * dt;

    if (progress_x >= end_x || progress_x < start_x) {
        progress_x = start_x;
    }

    Draw_Rectangle(grey, progress_x, 95, 37, 10, 10);

    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Image_Draw(player_opened, PLAYER_X_AXIS, 100, MINI_PLAYER_WIDTH, MINI_PLAYER_HEIGHT, 255);
    CP_Image_Draw(dumpling_opened, 630, 85, MINI_PLAYER_WIDTH + 10, MINI_PLAYER_HEIGHT + 10, 255);
    CP_Image_Draw(butter_opened, 950, 80, MINI_PLAYER_WIDTH + 15, MINI_PLAYER_HEIGHT + 15, 255);
    CP_Image_Draw(boss_opened, 1300, 85, MINI_PLAYER_WIDTH + 10, MINI_PLAYER_HEIGHT + 10, 255);
}

void Skill_Button(int round_finished) {
    int button_x = 220;
    float icon_x = 270;
    float cost_y = 865;

    if (round_finished == 1) {
        mana+=10;
        Trigger_Bubble_Animation();
        End_Bubble_Animation();
    }

    char text[50];
    snprintf(text, sizeof(text), "Mana: %d", mana);
    CP_Settings_Fill(white);
    CP_Settings_TextSize(50);
    CP_Font_DrawText(text, 100, 830);

    int skill_charge_amt = PLAYER_SPACESHIP_CHARGE;

    for (int i = SPACESHIP; i < SPACESHIP+3; i++) {
        bool spaceship_charged = (mana >= PLAYER_SPACESHIP_CHARGE);
        bool meteorite_charged = (mana >= PLAYER_METEORITE_CHARGE);
        bool bubble_charged = (mana >= PLAYER_BUBBLE_CHARGE);

        char cost[50];
        CP_Settings_TextSize(30);

		if (i == SPACESHIP) {
            if (spaceship_charged) {
                CP_Image_Draw(skill_button, icon_x, 800, 100, 100, 255);
                CP_Settings_Fill(dark_green);
                CP_Font_DrawText("Available!", icon_x, cost_y);
            }
            else {
                CP_Settings_Fill(white);
                snprintf(cost, sizeof(cost), "Cost: %d", skill_charge_amt);
                CP_Font_DrawText(cost, icon_x, cost_y);
                CP_Image_Draw(skill_button_grey, icon_x, 800, 100, 100, 255);
            }

			CP_Image_Draw(spaceship_icon, icon_x, 800, 150, 170, 255);
		}

		if (i == METEORITE) {
            skill_charge_amt = PLAYER_METEORITE_CHARGE;

            if (meteorite_charged) {
                CP_Image_Draw(skill_button, icon_x, 800, 100, 100, 255);
                CP_Settings_Fill(dark_green);
                CP_Font_DrawText("Available!", icon_x, cost_y);
            }
            else {
                CP_Settings_Fill(white);
                snprintf(cost, sizeof(cost), "Cost: %d", skill_charge_amt);
                CP_Font_DrawText(cost, icon_x, cost_y);
                CP_Image_Draw(skill_button_grey, icon_x, 800, 100, 100, 255);
            }

			CP_Image_Draw(meteorite_icon, icon_x, 800, 110, 120, 255);
		}

        if (i == BUBBLE) {
            skill_charge_amt = PLAYER_BUBBLE_CHARGE;

            if (bubble_charged) {
                CP_Image_Draw(skill_button, icon_x, 800, 100, 100, 255);
                CP_Settings_Fill(dark_green);
                CP_Font_DrawText("Available!", icon_x, cost_y);
            }
            else {
                CP_Settings_Fill(white);
                snprintf(cost, sizeof(cost), "Cost: %d", skill_charge_amt);
                CP_Font_DrawText(cost, icon_x, cost_y);
                CP_Image_Draw(skill_button_grey, icon_x, 800, 100, 100, 255);
            }

            CP_Image_Draw(bubble_icon, icon_x, 800, 100, 100, 255);
        }

        int clicked = Is_Area_Clicked(icon_x, 800, 100, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY());

        if (clicked) {
            if ((i == SPACESHIP) && spaceship_charged) {
                Trigger_Spaceship_Animation();
                enemy_damage_text_timer = DAMAGE_TEXT_DURATION;
                enemy_damage_text_value = (player.attack * 2) - current_enemy->defence;
                current_enemy->current_hp = Calculate_Current_HP(current_enemy->current_hp, (player.attack * 2), current_enemy->defence);
                score += (int)((player.attack * 2) - current_enemy->defence);
                mana -= PLAYER_SPACESHIP_CHARGE;
            }
            else if ((i == METEORITE) && meteorite_charged) {
                Trigger_Meteorite_Animation();
                enemy_damage_text_timer = DAMAGE_TEXT_DURATION;
                enemy_damage_text_value = (player.attack * 3) - current_enemy->defence;
                current_enemy->current_hp = Calculate_Current_HP(current_enemy->current_hp, (player.attack * 3), current_enemy->defence);
                score += (int)((player.attack * 3) - current_enemy->defence);
                mana -= PLAYER_METEORITE_CHARGE;
            } else if ((i == BUBBLE) && bubble_charged) {
                Trigger_Bubble_Animation();
                mana -= PLAYER_BUBBLE_CHARGE;
            }
        }

        button_x += 120;
        icon_x += 120;
    }
    
    Spaceship_Animation(spaceship_sprite);
    Meteorite_Animation(meteorite_sprite);
    Bubble_Animation(bubble_sprite);
}

int Boss_Ultimate(int current_round, float dt, bool* player_turn, int* enemy_basic_attack_shot) {
    static enum BossState {
        BOSS_DASH_FORWARD,
        BOSS_STUN_PAUSE,
        BOSS_DASH_BACK
    } boss_state = BOSS_DASH_FORWARD;

    static bool boss_ultimate_sound_played = false;
    
    float dash_speed = speed * 2.0f;
    
    if (boss_state == BOSS_DASH_FORWARD) {
        Check_Current_Enemy_State(EnemyState);
        // play boss ultimate sound once when boss moves towards player
        if (boss_ultimate_sound_played == false) {
            Audio_Play_Boss_Ultimate();
            boss_ultimate_sound_played = true;
        }

        enemy_x -= dash_speed * dt;
        bool boss_hit_player = Are_Circles_Intersecting(enemy_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, PLAYER_WIDTH / 2);

        if (!boss_hit_player) {
            EnemyState = ENEMY_ATTACKING;
        }
        else {
            player.current_hp = Calculate_Current_HP(player.current_hp, current_enemy->attack * 1.5f, player.defence);
            stunned_text_timer = DAMAGE_TEXT_DURATION;
            boss_state = BOSS_STUN_PAUSE;
        }
        return 0;
    }
    else if (boss_state == BOSS_STUN_PAUSE) {
        EnemyState = ENEMY_IDLE;
        boss_state = BOSS_DASH_BACK;
        CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
        Check_Current_Enemy_State(EnemyState);
        return 0;
    }
    else if (boss_state == BOSS_DASH_BACK) {
        enemy_x += dash_speed * dt;
        EnemyState = ENEMY_IDLE;
        
        if (enemy_x >= ENEMY_X_AXIS) {
            enemy_x = ENEMY_X_AXIS;
            boss_state = BOSS_DASH_FORWARD;

            // reset sound for the next boss ultimate
            boss_ultimate_sound_played = false;

            *player_turn = false;
            *enemy_basic_attack_shot = 0;
            return 1;
        }
        Check_Current_Enemy_State(EnemyState);
        return 0;
    }

    boss_state = BOSS_DASH_BACK;
    Check_Current_Enemy_State(EnemyState);
    return 0;
}

int Game_Per_Round(float dt, int current_round, int current_level) {
    int round_finished = 0;

    //setting background
    CP_Image_Draw(background, 800, 450, 1600, 900, 255);
    CP_Image_Draw(skip_button, 1500, 800, 100, 100, 255);
	CP_Settings_Fill(black);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    char speedMultiplierText[100];
    CP_Settings_TextSize(40);
    snprintf(speedMultiplierText, sizeof(speedMultiplierText), "x%d", speed_multiplier);
    CP_Font_DrawText(speedMultiplierText, 1500 , 720);

    if (Is_Area_Clicked(1500, 800, 100, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {

        Audio_Play_Button_Click();

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
    char round_text[50];
    snprintf(round_text, sizeof(round_text), "Round: %d/10", current_round);
    CP_Font_DrawText(round_text, 800, 130);   

    //--------------------------------------------PLAYER--------------------------------------------\\
    
    //player stats background
    CP_Settings_RectMode(CP_POSITION_CENTER);
    Draw_Rectangle(grey, 270, 410, 260, 95, 10);

    //player stat icons
    CP_Image_Draw(defence_icon, 165, PLAYER_Y_AXIS - PLAYER_HEIGHT - 40, 25, 25, 255);
    CP_Image_Draw(fireball, 165, PLAYER_Y_AXIS - PLAYER_HEIGHT - 10, 25, 25, 255);
    CP_Image_Draw(hp_icon, 165, PLAYER_Y_AXIS - PLAYER_HEIGHT + 20, 25, 25, 255);

    //defence text
    Text(30, white, player.defence, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT - 40, "Defence: %.2f");

    //attack text
    Text(30, white, player.attack, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT - 10, "Attack: %.2f");

    //max hp text
    Text(30, white, player.max_hp, PLAYER_X_AXIS - 20, PLAYER_Y_AXIS - PLAYER_HEIGHT + 20, "Max HP: %.2f");
    //--------------------------------------------PLAYER--------------------------------------------\\

    //--------------------------------------------ENEMY--------------------------------------------\\
    
    //enemy stats background
    CP_Settings_RectMode(CP_POSITION_CENTER);
    Draw_Rectangle(grey, 1300, 480, 260, 95, 10);

    //enemy stat icons
    CP_Image_Draw(defence_icon, 1195, ENEMY_Y_AXIS - ENEMY_HEIGHT, 25, 25, 255);
    CP_Image_Draw(fireball, 1195, ENEMY_Y_AXIS - ENEMY_HEIGHT + 30, 25, 25, 255);
    CP_Image_Draw(hp_icon, 1195, ENEMY_Y_AXIS - ENEMY_HEIGHT + 60, 25, 25, 255);
 
    //defence text
    Text(30, white, current_enemy->defence, ENEMY_X_AXIS + 10, ENEMY_Y_AXIS - ENEMY_HEIGHT, "Defence: %.2f");

    //attack text
    Text(30, white, current_enemy->attack, ENEMY_X_AXIS + 10, ENEMY_Y_AXIS - ENEMY_HEIGHT + 30, "Attack: %.2f");

    //max hp text
    Text(30, white, current_enemy->max_hp, ENEMY_X_AXIS + 10, ENEMY_Y_AXIS - ENEMY_HEIGHT + 60, "Max HP: %.2f");
    //--------------------------------------------ENEMY--------------------------------------------\\

    //current level status, progress bar
    Progress_Bar(current_level, dt);

	//--------------------------------------------PLAYER ATTACK--------------------------------------------//
    if (player_turn == true) {

        if (player_basic_attack_shot < player.skills[BASIC_ATTACK].iteration) {
            //collision check for player fireball and enemy
            bool fireballHitEnemy = Are_Circles_Intersecting(fireball_x, PLAYER_Y_AXIS, 90 / 2, ENEMY_X_AXIS, ENEMY_Y_AXIS, PLAYER_WIDTH / 2);

            // play sound when player launches a new fireball
            if (fireball_x == 300) {
                Audio_Play_Player_Projectile();
            }

            //if not colliding, fireball continues to travel forward. else, enemy's hp is reduced based off player's attack
            if (!fireballHitEnemy) {
                fireball_x += speed * dt;
                CP_Image_Draw(fireball, fireball_x, PLAYER_Y_AXIS, 90, 90, 255);
				PlayerState = PLAYER_ATTACKING;
            }
            else {
                enemy_damage_text_timer = DAMAGE_TEXT_DURATION;
                enemy_damage_text_value = player.attack - current_enemy->defence;

                //base health should be current enemy health, and base attack should be current player attack
                // update current hp with reduced health
                current_enemy->current_hp = Calculate_Current_HP(current_enemy->current_hp, player.attack, current_enemy->defence);
                
                fireball_x = 300; // reset fireball position after hitting enemy
                
                player_basic_attack_shot++;
                score += (int)(player.attack - current_enemy->defence);
                PlayerState = PLAYER_IDLE;
            }

            
        }
        else {
            player_basic_attack_shot = 0;
            player_turn = false;
        }
        
    }
    //--------------------------------------------PLAYER ATTACK--------------------------------------------//

    Check_Current_Player_State(PlayerState);

    //--------------------------------------------ENEMY ATTACK--------------------------------------------//
    if (player_turn == false) {
        //collision check for enemy soundwave and player
        bool soundwaveHitPlayer = Are_Circles_Intersecting(soundwave_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, ENEMY_WIDTH / 2);
        bool enemyHitPlayer = Are_Circles_Intersecting(enemy_x, ENEMY_Y_AXIS, 90 / 2, PLAYER_X_AXIS, PLAYER_Y_AXIS, ENEMY_WIDTH / 2);

        //enemy has not done all of its basic attack shots
        if (enemy_basic_attack_shot < current_enemy->skills[BASIC_ATTACK].iteration) {

            // play sound when enemy launches a new soundwave
            if (soundwave_x == 1300) {
                Audio_Play_Enemy_Soundwave();
            }
            
            //if not colliding, soundwave continues to travel forward. else, player's hp is reduced based off enemy's attack
            if (!soundwaveHitPlayer) {
                soundwave_x -= speed * dt;
                CP_Image_Draw(soundwave, soundwave_x, ENEMY_Y_AXIS, 90, 90, 255);
                EnemyState = ENEMY_ATTACKING;
			}
			else {
				EnemyState = ENEMY_IDLE;

				if (Is_Bubble_Animation_Active()) {
					player_damage_text_timer = DAMAGE_TEXT_DURATION;
					player_damage_text_value = 0.0f;
				} else {
					player_damage_text_timer = DAMAGE_TEXT_DURATION;
					player_damage_text_value = current_enemy->attack - player.defence;
					// update current hp with reduced health
					player.current_hp = Calculate_Current_HP(player.current_hp, current_enemy->attack, player.defence);
					score -= (int)(current_enemy->attack - player.defence);
				}

				soundwave_x = 1300; // reset soundwave position after hitting player
				++enemy_basic_attack_shot;
			}
            
        }
        //enemy has done all basic attack shots
        else {
            //if current enemy is a boss and current round is divisible by ultimate charge, trigger boss ultimate
            if (current_enemy == &boss && (current_round % boss.skills[ULTIMATE].charge == 0)) {
                return Boss_Ultimate(current_round, dt, &player_turn, &enemy_basic_attack_shot);
            }
            //current enemy is not boss or current round is not divisible by ultimate charge, resets enemy's basic attack,
            //changes back to player's turn, and increments round
            else {
                EnemyState = ENEMY_IDLE;
                enemy_basic_attack_shot = 0;
                player_turn = true;
                round_finished = 1;
            }
        }
    }
    
    Check_Current_Enemy_State(EnemyState);
    //--------------------------------------------ENEMY ATTACK--------------------------------------------//

    Health_Bar(dark_red, 1250, 530, current_enemy->current_hp, current_enemy->max_hp);
    Health_Bar(dark_green, 230, PLAYER_Y_AXIS - PLAYER_HEIGHT + 50, player.current_hp, player.max_hp);
    Skill_Button(round_finished);

    if (player_damage_text_timer > 0.0f) {
        player_damage_text_timer -= dt;


        if (player_damage_text_value == 0.0f && Is_Bubble_Animation_Active()) {
            Fading_Text(
                player_damage_text_timer,
                DAMAGE_TEXT_DURATION,
                "Blocked!",
                PLAYER_X_AXIS + 100,
                PLAYER_Y_AXIS + 50
            );
        } else {
            Damage_Text(
                player_damage_text_timer, 
                DAMAGE_TEXT_DURATION, 
                player_damage_text_value,
                PLAYER_X_AXIS + 100,
                PLAYER_Y_AXIS + 50
            );
        }
    }

    //display damage after hitting enemy
    if (enemy_damage_text_timer > 0.0f) {
        enemy_damage_text_timer -= dt;

        Damage_Text(
            enemy_damage_text_timer,
            DAMAGE_TEXT_DURATION,
            enemy_damage_text_value,
            ENEMY_X_AXIS - 110,
            ENEMY_Y_AXIS + 50
        );
    }

    //display damage after hitting enemy
    if (stunned_text_timer > 0.0f) {
        stunned_text_timer -= dt;

        Fading_Text(
            stunned_text_timer,
            DAMAGE_TEXT_DURATION,
            "STUNNED!",
            PLAYER_X_AXIS + 100,
            PLAYER_Y_AXIS + 100
        );
    }

    return round_finished;
}

void Reset_Characters(void)
{
    // reset player skill charge
    mana = 0;
    
    // player
    player.attack = PLAYER_ATTACK;
    player.defence = PLAYER_DEFENCE;
    player.current_hp = PLAYER_CURRENT_HP;
    player.max_hp = PLAYER_MAX_HP;
	player.skills[BASIC_ATTACK].iteration = PLAYER_BASIC_ATTACK_ITERATION;

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
    player_opened = CP_Image_Load("./Assets/player-open.png");
    dumpling_opened = CP_Image_Load("./Assets/dumpling-open.png");
    butter_opened = CP_Image_Load("./Assets/butter-open.png");
    boss_opened = CP_Image_Load("./Assets/boss-open.png");
    fireball = CP_Image_Load("./Assets/fireball.png");
    soundwave = CP_Image_Load("./Assets/soundwave.png");
	skip_button = CP_Image_Load("./Assets/skip-button.png");
    defence_icon = CP_Image_Load("./Assets/defence-icon.png");
    hp_icon = CP_Image_Load("./Assets/hp-icon.png");

    player_sprite = CP_Image_Load("./Assets/player-sprite.png");
    dumpling_sprite = CP_Image_Load("./Assets/dumpling-sprite.png");
    butter_sprite = CP_Image_Load("./Assets/butter-sprite.png");
    boss_sprite = CP_Image_Load("./Assets/boss-sprite.png");
    spaceship_sprite = CP_Image_Load("./Assets/spaceship-sprite.png");
    spaceship_icon = CP_Image_Load("./Assets/spaceship-icon.png");
    meteorite_sprite = CP_Image_Load("./Assets/meteorite-sprite.png");
    meteorite_icon = CP_Image_Load("./Assets/meteorite-icon.png");
    bubble_sprite = CP_Image_Load("./Assets/bubble-sprite.png");
    bubble_icon = CP_Image_Load("./Assets/bubble-icon.png");
    skill_button = CP_Image_Load("./Assets/skill-button.png");
    skill_button_grey = CP_Image_Load("./Assets/skill-button-grey.png");

    score = 0; // reset score every time new game
    skip_button = CP_Image_Load("./Assets/skip-button.png");

    game_over = false; // reset game over state for a new game
     
    dark_red = CP_Color_Create(255, 0, 0, 255);
    dark_green = CP_Color_Create(0, 255, 0, 255);
    grey = CP_Color_Create(70, 70, 70, 225);
    light_grey = CP_Color_Create(100, 100, 100, 100);
    black = CP_Color_Create(0, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);

    // start fade in
    game_fade = 255.0f;

    enemy_damage_text_timer = 0.0f;
    player_damage_text_timer = 0.0f;

    enemy_x = 1300;

    // play background music for current level
    Audio_Play_Level(current_level);

    Debug_Init();
    Pause_Menu_Init();
}

void Game_Update(void)
{
    //get delta time
    float dt = CP_System_GetDt();

    Check_Current_Enemy(current_level);

    if (current_enemy->current_hp <= 0 &&
        current_round <= 10 &&
        player.current_hp > 0 &&
        !upgrade_triggered && current_level < 4) {

        show_upgrade = true;
        upgrade_triggered = true;
        ++current_level;
    }

    if (current_level > 3) {
        if (highscore < score) {
            highscore = score;
        }

        // player won
        Set_End_Result(true);

        CP_Engine_SetNextGameState(End_Menu_Init, End_Menu_Update, End_Menu_Exit);

        current_level = 1;
        current_round = 1;
        show_upgrade = false;
        upgrade_triggered = false;
        Reset_Characters();
        Check_Current_Enemy(current_level);
    }

    else if (show_upgrade) {
        CP_Engine_SetNextGameState(Upgrades_Init, Upgrades_Update, Upgrades_Exit);
        current_round = 1;
        show_upgrade = false;
        upgrade_triggered = false;
        return;
    }

    // player loses if they have lesser hp than dumpling
    if (player.current_hp <= 0 && current_enemy->current_hp > 0)
    {
        game_over = true;

        if (highscore < score)
        {
            highscore = score;
        }

        // player lost
        Set_End_Result(false);

        CP_Engine_SetNextGameState(End_Menu_Init, End_Menu_Update, End_Menu_Exit);

        current_level = 1;
        current_round = 1;
        show_upgrade = false;
        upgrade_triggered = false;

        Reset_Characters();
        Check_Current_Enemy(current_level);

        return;
    }
    // if current round exceeds 10, game is over
    if (current_round > 10)
    {
        game_over = true;

        if (highscore < score)
        {
            highscore = score;
        }

        // player lost
        Set_End_Result(false);

        CP_Engine_SetNextGameState(End_Menu_Init, End_Menu_Update, End_Menu_Exit);

        current_level = 1;
        current_round = 1;
        show_upgrade = false;
        upgrade_triggered = false;

        Reset_Characters();
        Check_Current_Enemy(current_level);

        return;
    }

    // debug 
    Debug_Update();

    // update the round for each hit when game is not over
    if (!is_paused && !game_over && current_round <= 10) {
        float active_dt = Debug_Should_Update_Game() ? dt : 0.0f;

        if (Game_Per_Round(active_dt, current_round, current_level) == 1) {
            ++current_round;
        }

        Debug_Record_Frame();
    }


    Debug_Draw_Collision_Circles();
    // pause 
    Pause_Menu_Update();
    Debug_Draw();

    // fade in effect
    if (game_fade > 0.0f)
    {
        game_fade -= 510.0f * dt;

        if (game_fade < 0.0f)
        {
            game_fade = 0.0f;
        }

        CP_Settings_RectMode(CP_POSITION_CENTER);
        Draw_Rectangle(CP_Color_Create(0, 0, 0, (int)game_fade), 800, 450, 1600, 900, 0);
    }
}

void Game_Exit(void)
{
    CP_Image_Free(player_opened);
    CP_Image_Free(dumpling_opened);
    CP_Image_Free(butter_opened);
    CP_Image_Free(boss_opened);
	CP_Image_Free(fireball);
	CP_Image_Free(soundwave);
    Pause_Menu_Exit();
}