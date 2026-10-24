#pragma once
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
void DrawRectangle(CP_Color colour, float x, float y, float width, float height);
double CalculateCurrentAttack(double increment_percentage);
double CalculateCurrentDefence(double increment_percentage);
double CalculateCurrentMaxHP(double increment_percentage);
