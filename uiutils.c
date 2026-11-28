#include <math.h>
#include "cprocessing.h"
#include "uiutils.h"
#include "constants.h"
#include "upgrades.h"
#include "game.h"


void Draw_Rectangle(CP_Color colour, float x, float y, float width, float height, float corner_radius) {
	CP_Settings_Fill(colour);
	CP_Graphics_DrawRectAdvanced(x, y, width, height, 0.0f, corner_radius);
}

void Draw_Circle(float x, float y, float diameter, char* text, CP_Color colour) {
	CP_Settings_Fill(colour);
	CP_Graphics_DrawCircle(x, y, diameter);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(text, x, y);
}

void Skill_Upgrade_Button(
	int skillEnum,
	float image_x, float image_y, float image_width, float image_height,
	CP_Color colour,
	char* description_text, float description_text_x, float description_text_y,
	CP_Image icon, float icon_x, float icon_y, float icon_width, float icon_height,
	float value, CP_Color circle_colour, int rarity
) {
	CP_Image common_button = CP_Image_Load("./Assets/defence-button.png");
	CP_Image rare_button = CP_Image_Load("./Assets/attack-button.png");
	CP_Image very_rare_button = CP_Image_Load("./Assets/hp-button.png");
	
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
		Draw_Circle(icon_x + 80, icon_y + 40, 60, value_text, circle_colour);
	}
	
	CP_Settings_Fill(colour);
	CP_Settings_TextSize(40);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawTextBox(description_text, description_text_x, description_text_y, 250);
}

void Health_Bar(CP_Color current_colour, float x, float y, float current_hp, float max_hp) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_NoStroke();
	Draw_Rectangle(CP_Color_Create(100, 100, 100, 255), x, y, 100, 20, 0);
	if (current_hp > 0) Draw_Rectangle(current_colour, x, y, (current_hp/max_hp)*100, 20, 0);
}

void Text(float font_size, CP_Color colour, double value, float text_x, float text_y, const char*const string) {
	CP_Settings_TextSize(font_size);
	CP_Settings_Fill(colour);
	char text[50];
	snprintf(text, sizeof(text), string, value);
	CP_Font_DrawText(text, text_x, text_y);
}

void Damage_Text(
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

void Fading_Text(
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