#include "cprocessing.h"
#include "utils.h"

void Upgrades_Init(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
}

void Upgrades_Update(void) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Color pink = CP_Color_Create(100, 0, 0, 255);
	DrawRectangle(pink, 450, 450, 300, 500);
	DrawRectangle(pink, 800, 450, 300, 500);
	DrawRectangle(pink, 1150, 450, 300, 500);
}

void Upgrades_Exit(void) {

}