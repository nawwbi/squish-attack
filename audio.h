#pragma once

#include "cprocessing.h"
#include <stdbool.h>

// load and free all audio
void Audio_Load(void);
void Audio_Free(void);

// background music
void Audio_Play_Main_Menu(void);
void Audio_Play_Level(int level);
void Audio_Play_End_Screen(bool player_won);

// sound effects
void Audio_Play_Button_Click(void);
void Audio_Play_Button_Hover(void);
void Audio_Play_Player_Projectile(void);
void Audio_Play_Enemy_Soundwave(void);
void Audio_Play_Boss_Ultimate(void);

// stop music or all audio
void Audio_Stop_Music(void);
void Audio_Stop_All(void);
