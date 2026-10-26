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

void DrawRectangle(CP_Color colour, float x, float y, float width, float height, float cornerRadius) {
	CP_Settings_Fill(colour);
	CP_Graphics_DrawRectAdvanced(x, y, width, height, 0.0f, cornerRadius);
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

double CalculateDamage(double attack, double defence)
{
	double current_damage = attack - defence;
	return current_damage;
}


double CalculateCurrentAttack(double attack, double increment_percentage)
{
	double current_attack = attack * (1 + increment_percentage);
	return current_attack;
}

double CalculateCurrentDefence(double defence, double increment_percentage)
{
	double current_defence = defence * (1 + increment_percentage);
	return current_defence;
}

// calculates current hp based on damage taken
double CalculateCurrentHP(double previous_hp, double received_damage)
{

	double current_hp = previous_hp - received_damage;

	if (current_hp < 0.0) // makes sure that the current hp dont go below 0
	{
		current_hp = 0.0;
	}

	return current_hp;
}

double CalculateCurrentMaxHP(double health , double increment_percentage)
{
	double current_max_hp = health * (1 + increment_percentage);
	// (suggestion!) add that current hp gets updated too when player picks the health upgrade
	return current_max_hp;
}

void SkillUpgradeButton(
	CP_Image image, float image_x, float image_y, float image_width, float image_height,
	CP_Color colour, 
	CP_Font font,
	char* text, float text_x, float text_y
) {
	CP_Image_Draw(image, image_x, image_y, image_width, image_height, 255);
	CP_Settings_Fill(colour);
	CP_Settings_TextSize(45);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_Set(font);
	CP_Font_DrawTextBox(text, text_x, text_y, 250);
}

void HealthBar(CP_Color current_colour, float x, float y, float current_hp, float max_hp) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_NoStroke();
	DrawRectangle(CP_Color_Create(100, 100, 100, 255), x, y, 100, 20, 0);
	if (current_hp > 0) DrawRectangle(current_colour, x, y, (current_hp/max_hp)*100, 20, 0);
}

bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	double distance_squared = (((c1_x - c2_x) * (c1_x - c2_x)) + ((c1_y - c2_y) * (c1_y - c2_y)));
	double diameter_squared = (r1 + r2) * (r1 + r2);
	return distance_squared <= diameter_squared;
}