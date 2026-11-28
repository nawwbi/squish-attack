#pragma once
#include <stdbool.h>

void Character_Animation(CP_Image sprite, bool is_player, bool top, float x);

void Spaceship_Animation(CP_Image sprite);
void Meteorite_Animation(CP_Image sprite);
void Bubble_Animation(CP_Image sprite);

void Trigger_Spaceship_Animation(void);
void Trigger_Meteorite_Animation(void);
void Trigger_Bubble_Animation(void);

void End_Bubble_Animation(void);
bool Is_Bubble_Animation_Active(void);