#pragma once
#include <stdbool.h>

int Is_Area_Clicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

void Draw_Rectangle(CP_Color colour, float x, float y, float width, float height, float cornerRadius);
void Draw_Circle(float x, float y, float diameter, char* text, CP_Color colour);

float Calculate_Healing(float previous_hp, float max_hp, float healing_percentage);
float Calculate_Current_Attack(float attack, float increment_percentage);
float Calculate_Current_Defence(float defence, float increment_percentage);
float Calculate_Current_HP(float previous_hp, float received_damage, float defence);
float Calculate_Current_Max_HP(float health, float increment_percentage);

void Skill_Upgrade_Button(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* text, float text_x, float text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height,
	float value, CP_Color circle_colour, int rarity
);

void Health_Bar(CP_Color current_colour, float x, float y, float current_hp, float max_hp);

bool Are_Circles_Intersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

void Text(float font_size, CP_Color colour, double value, float text_x, float text_y, const char* const string);

void Damage_Text(
	float current_timer, float total_timer, float value,
	float x, float y
);

void Fading_Text(
	float current_timer, float total_timer, char* string,
	float x, float y
);