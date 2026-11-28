#pragma once
#include <stdbool.h>

float Calculate_Healing(float previous_hp, float max_hp, float healing_percentage);
float Calculate_Current_Attack(float attack, float increment_percentage);
float Calculate_Current_Defence(float defence, float increment_percentage);
float Calculate_Current_HP(float previous_hp, float received_damage, float defence);
float Calculate_Current_Max_HP(float health, float increment_percentage);