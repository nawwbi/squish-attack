#pragma once

// existing skill enums: basic attack, ultimate
typedef enum SkillEnum {
	BASIC_ATTACK,
	ULTIMATE,
	SPACESHIP,
	METEORITE,
	BUBBLE
} SkillEnum;

typedef enum Rarity {
	COMMON = 85,
	RARE = 10,
	VERY_RARE = 5
} Rarity;

// int skill_enum, int iteration, char* details
typedef struct Skills {
	enum SkillEnum skill_enum;
	int iteration;
	int charge; // only used for ultimate skill
} Skills;

struct Characters {
	float attack;
	float defence;
	float current_hp;
	float max_hp;
	Skills skills [];
} player, dumpling, butter, boss;

const float PLAYER_WIDTH;
const float PLAYER_HEIGHT;
const float PLAYER_X_AXIS;
const float PLAYER_Y_AXIS;

const float ENEMY_X_AXIS;
const float ENEMY_Y_AXIS;
const float ENEMY_WIDTH;
const float ENEMY_HEIGHT;

const float BUTTON_WIDTH;
const float BUTTON_HEIGHT;
const float BUTTON_X_AXIS;
const float BUTTON_Y_AXIS;

const float MINI_PLAYER_WIDTH;
const float MINI_PLAYER_HEIGHT;

const float ICON_WIDTH;
const float ICON_HEIGHT;
const float ICON_X_AXIS;
const float ICON_Y_AXIS;

extern int score;
extern int highscore;