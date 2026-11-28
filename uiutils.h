#pragma once
#include <stdbool.h>

void Draw_Rectangle(CP_Color colour, float x, float y, float width, float height, float corner_radius);
void Draw_Circle(float x, float y, float diameter, char* text, CP_Color colour);

void Skill_Upgrade_Button(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* text, float text_x, float text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height,
	float value, CP_Color circle_colour, int rarity
);

void Health_Bar(CP_Color current_colour, float x, float y, float current_hp, float max_hp);

void Text(float font_size, CP_Color colour, double value, float text_x, float text_y, const char* const string);

void Damage_Text(
	float current_timer, float total_timer, float value,
	float x, float y
);

void Fading_Text(
	float current_timer, float total_timer, char* string,
	float x, float y
);