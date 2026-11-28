#include <math.h>
#include "cprocessing.h"
#include "mathutils.h"
#include "constants.h"
#include "upgrades.h"
#include "game.h"

int Is_Area_Clicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
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

bool Are_Circles_Intersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	double distance_squared = (((c1_x - c2_x) * (c1_x - c2_x)) + ((c1_y - c2_y) * (c1_y - c2_y)));
	double diameter_squared = (r1 + r2) * (r1 + r2);
	return distance_squared <= diameter_squared;
}