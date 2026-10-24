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

double CalculateCurrentMaxHP(double increment_percentage)
{
	double current_maxHP = BASE_HEALTH * (1 + increment_percentage);
	return current_maxHP;
}
