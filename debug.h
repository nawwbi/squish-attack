#pragma once
#include "cprocessing.h"
#include <stdbool.h>

extern bool debug_mode;

void Debug_Init(void);
void Debug_Update(void);
void Debug_Draw(void);

// true if game runs normally, false if frozen
bool Debug_ShouldUpdateGame(void);

// call after Game_Per_Round to save a snapshot for frame rewinding
void Debug_RecordFrame(void);

/*
void Debug_StartTimer(int section); // 0 = combat logic, 1 = UI & Menus
void Debug_EndTimer(int section);
*/

// draws collision circles when debug_mode & hitboxes are on
void Debug_DrawCollisionCircles(void);