#pragma once
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
void DrawRectangle(CP_Color colour, float x, float y, float width, float height, float cornerRadius);
double CalculateCurrentAttack(double increment_percentage);
double CalculateCurrentDefence(double increment_percentage);
double CalculateCurrentHP(double previous_hp, double received_damage);
double CalculateCurrentMaxHP(double increment_percentage);

void SkillUpgradeButton(
	CP_Image image, float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	CP_Font font,
	char* text, float text_x, float text_y
);

void HealthBar(CP_Color current_colour, float x, float y, float current_hp, float max_hp);