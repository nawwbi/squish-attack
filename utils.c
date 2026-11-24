#include <math.h>
#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "upgrades.h"
#include "game.h"

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

void DrawCircle(float x, float y, float diameter, char* text, CP_Color colour) {
	CP_Settings_Fill(colour);
	CP_Graphics_DrawCircle(x, y, diameter);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(text, x, y);
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
double CalculateCurrentHP(double previous_hp, double received_damage, double defence)
{
	received_damage = received_damage - defence;

	if (received_damage < 0) {
		received_damage = 0;
	}

	double current_hp = previous_hp - received_damage;

	return current_hp;
}

double CalculateCurrentMaxHP(double health , double increment_percentage)
{
	double current_max_hp = health * (1 + increment_percentage);
	// (suggestion!) add that current hp gets updated too when player picks the health upgrade
	return current_max_hp;
}

void SkillUpgradeButton(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* description_text, float description_text_x, float description_text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height,
	float value, CP_Color circle_colour, int rarity
) {
	CP_Image common_button = CP_Image_Load("./Assets/defence_button.png");
	CP_Image rare_button = CP_Image_Load("./Assets/attack_button.png");
	CP_Image very_rare_button = CP_Image_Load("./Assets/hp_button.png");
	
	CP_Image_Draw(
		(rarity == VERY_RARE) ? very_rare_button : 
		(rarity == RARE) ? rare_button :
		common_button, 
		
		(rarity == RARE) ? (image_x + 10) : image_x,
		
		(rarity == RARE) ? (image_y - 23) : image_y,
		
		image_width, image_height, 255);
	CP_Image_Draw(icon, icon_x, icon_y, icon_width, icon_height, 255);

	char value_text[3];
	if (skillEnum == BASIC_ATTACK_ITERATION && (value >= 1)) {
		snprintf(value_text, sizeof(value_text), "%+f", value); //auto positive numbers has a +
		DrawCircle(icon_x + 80, icon_y + 40, 60, value_text, circle_colour);
	}
	
	CP_Settings_Fill(colour);
	CP_Settings_TextSize(40);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawTextBox(description_text, description_text_x, description_text_y, 250);
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

void Text(int font_size, CP_Color colour, double value, float text_x, float text_y, const char*const string) {
	CP_Settings_TextSize(font_size);
	CP_Settings_Fill(colour);
	char text[50];
	snprintf(text, sizeof(text), string, value);
	CP_Font_DrawText(text, text_x, text_y);
}

void DamageText(
	float current_timer, float total_timer, float value,
	float x, float y
) {
	//fading out
	int alpha = (int)(255 * (current_timer / total_timer));
	if (alpha < 0) alpha = 0;

	//floating up
	float rise = (total_timer - current_timer) * 60.0f;

	char text[50];
	snprintf(text, sizeof(text), "-%.0f", value);
	CP_Settings_Fill(CP_Color_Create(230, 50, 50, alpha));
	CP_Settings_TextSize(80);
	CP_Font_DrawText(text, x, y - rise);
}

void FadingText(
	float current_timer, float total_timer, char* string,
	float x, float y
) {
	//fading out
	int alpha = (int)(255 * (current_timer / total_timer));
	if (alpha < 0) alpha = 0;

	//floating up
	float rise = (total_timer - current_timer) * 60.0f;

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
	CP_Settings_TextSize(80);
	CP_Font_DrawText(string, x, y - rise);
}