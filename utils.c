#include <math.h>
#include "cprocessing.h"
#include "utils.h"
#include "constants.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float half_width = area_width / 2;
	float half_height = area_height / 2;

	float left = area_center_x - half_width;
	float right = area_center_x + half_width;
	float top = area_center_y - half_height;
	float bottom = area_center_y + half_height;

	if (click_x >= left && click_x <= right && click_y >= top && click_y <= bottom && CP_Input_MouseClicked())
	{
		return 1;
	}
	else { return 0; }
}

// calculates current hp based on healing recieved 
double CalculateHealing(double previous_hp, double max_hp, double healing_percentage)
{
	double current_hp = previous_hp + (max_hp * healing_percentage);
	if (current_hp > max_hp) // make sure that current hp dont go above max hp
	{
		current_hp = max_hp;
	}
	return current_hp;
}

double CalculateDamage(double attack, double defense)
{
	double current_damage = attack - defense;
	return current_damage;
}

double CalculateCurrentAttack(double increment_percentage)
{
	double current_attack = BASE_ATTACK * (1 + increment_percentage);
	return current_attack;
}

double CalculateCurrentDefence(double increment_percentage)
{
	double current_defence = BASE_DEFENCE * (1 + increment_percentage);
	return current_defence;
}

// calculates current hp based on damage taken
double CalculateCurrentHP(double previous_hp, double max_hp, double received_damage)
{

	double current_HP = previous_hp - received_damage;

	if (current_HP < 0.0) // makes sure that the current hp dont go below 0
	{
		current_HP = 0.0;
	}

	return current_HP;
}

double CalculateCurrentMaxHP(double increment_percentage)
{
	double current_maxHP = BASE_HEALTH * (1 + increment_percentage);
	// (suggestion!) add that current hp gets updated too when player picks the health upgrade
	return current_maxHP;
}
