#include <math.h>
#include "cprocessing.h"
#include "combatutils.h"
#include "constants.h"
#include "upgrades.h"
#include "game.h"

// calculates current hp based on healing recieved 
float Calculate_Healing(float previous_hp, float max_hp, float healing_percentage)
{
	float current_hp = previous_hp + (max_hp * healing_percentage);
	if (current_hp > max_hp) // make sure that current hp dont go above max hp
	{
		current_hp = max_hp;
	}
	return current_hp;
}

float Calculate_Damage(float attack, float defence)
{
	float current_damage = attack - defence;
	return current_damage;
}


float Calculate_Current_Attack(float attack, float increment_percentage)
{
	float current_attack = attack * (1 + increment_percentage);
	return current_attack;
}

float Calculate_Current_Defence(float defence, float increment_percentage)
{
	float current_defence = defence * (1 + increment_percentage);
	return current_defence;
}

// calculates current hp based on damage taken
float Calculate_Current_HP(float previous_hp, float received_damage, float defence)
{
	received_damage = received_damage - defence;

	if (received_damage < 0) {
		received_damage = 0;
	}

	float current_hp = previous_hp - received_damage;

	return current_hp;
}

float Calculate_Current_Max_HP(float health, float increment_percentage)
{
	float current_max_hp = health * (1 + increment_percentage);
	// (suggestion!) add that current hp gets updated too when player picks the health upgrade
	return current_max_hp;
}