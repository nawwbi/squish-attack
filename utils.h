#pragma once
#include <stdbool.h>

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

void DrawRectangle(CP_Color colour, float x, float y, float width, float height, float cornerRadius);
void DrawCircle(float x, float y, float diameter, char* text, CP_Color colour);

float CalculateHealing(float previous_hp, float max_hp, float healing_percentage);
float CalculateCurrentAttack(float attack, float increment_percentage);
float CalculateCurrentDefence(float defence, float increment_percentage);
float CalculateCurrentHP(float previous_hp, float received_damage, float defence);
float CalculateCurrentMaxHP(float health, float increment_percentage);

void SkillUpgradeButton(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* text, float text_x, float text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height,
	float value, CP_Color circle_colour, int rarity
);

void HealthBar(CP_Color current_colour, float x, float y, float current_hp, float max_hp);

bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void Text(float font_size, CP_Color colour, double value, float text_x, float text_y, const char* const string);

void DamageText(
	float current_timer, float total_timer, float value,
	float x, float y
);

void FadingText(
	float current_timer, float total_timer, char* string,
	float x, float y
);