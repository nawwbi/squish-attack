#pragma once

// existing skill enums: basic attack, ultimate
typedef enum SkillEnum {
	BASIC_ATTACK,
	ULTIMATE
} SkillEnum;

// int skill_enum, int iteration, char* details
typedef struct Skills {
	int skill_enum;
	int iteration;
	int charge; // only used for ultimate skill
} Skills;

// double attack, double defence, double current_hp, double max_hp
// Skills skills []
struct Characters {
	double attack;
	double defence;
	double current_hp;
	double max_hp;
	Skills skills [];
} player, dumpling, butter, boss;

const double PLAYER_WIDTH;
const double PLAYER_HEIGHT;
const double PLAYER_X_AXIS;
const double PLAYER_Y_AXIS;

const double ENEMY_X_AXIS;
const double ENEMY_Y_AXIS;
const double ENEMY_WIDTH;
const double ENEMY_HEIGHT;

const double BUTTON_WIDTH;
const double BUTTON_HEIGHT;
const double BUTTON_X_AXIS;
const double BUTTON_Y_AXIS;

const double MINI_PLAYER_WIDTH;
const double MINI_PLAYER_HEIGHT;

const double ICON_WIDTH;
const double ICON_HEIGHT;
const double ICON_X_AXIS;
const double ICON_Y_AXIS;

extern int score;
extern int highscore;