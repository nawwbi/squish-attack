#include "cprocessing.h"
#include "levelintro.h"
#include "game.h"
#include "audio.h"
#include "debug.h"

extern int current_level;

float level_intro_timer = 0.0f;

void Level_Intro_Init(void)
{
    level_intro_timer = 0.0f;

    // stop previous level music during next level intro
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}

void Level_Intro_Update(void)
{
    float dt = CP_System_GetDt();

    level_intro_timer += dt;

    // black background
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    // text settings
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

    if (current_level == 1)
    {
        CP_Settings_TextSize(70);
        CP_Font_DrawText("Level 1", 800.0f, 420.0f);

        CP_Settings_TextSize(40);
        CP_Font_DrawText("Dumpling", 800.0f, 490.0f);
    }
    else if (current_level == 2)
    {
        CP_Settings_TextSize(70);
        CP_Font_DrawText("Level 2", 800.0f, 420.0f);

        CP_Settings_TextSize(40);
        CP_Font_DrawText("Butter", 800.0f, 490.0f);
    }
    else if (current_level == 3)
    {
        CP_Settings_TextSize(70);
        CP_Font_DrawText("Level 3", 800.0f, 420.0f);

        CP_Settings_TextSize(40);
        CP_Font_DrawText("Boss", 800.0f, 490.0f);
    }

    // start level after 1.5 seconds
    if (level_intro_timer >= 1.5f)
    {
        CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
    }

    Debug_Update();
    Debug_Draw();
}

void Level_Intro_Exit(void)
{
}