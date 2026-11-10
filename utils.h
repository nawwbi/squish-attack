#pragma once
#include <stdbool.h>

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
void DrawRectangle(CP_Color colour, float x, float y, float width, float height, float cornerRadius);
double CalculateHealing(double previous_hp, double max_hp, double healing_percentage);
double CalculateCurrentAttack(double attack, double increment_percentage);
double CalculateCurrentDefence(double defence, double increment_percentage);
double CalculateCurrentHP(double previous_hp, double received_damage, double defence);
double CalculateCurrentMaxHP(double health, double increment_percentage);

void SkillUpgradeButton(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* text, float text_x, float text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height
);

void HealthBar(CP_Color current_colour, float x, float y, float current_hp, float max_hp);

bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void Text(int font_size, CP_Color colour, double value, float text_x, float text_y, const char* const string);