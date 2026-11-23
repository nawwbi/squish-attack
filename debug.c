#include "cprocessing.h"
#include "utils.h"
#include "constants.h"
#include "game.h"
#include "audio.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool debug_mode = false;
static bool show_hitboxes = true;
static bool is_frozen = false;
static bool step_forward = false;


//static float timer_start[2] = { 0.0f, 0.0f };
//static float timer_result_ms[2] = { 0.0f, 0.0f };


#define MAX_FRAMES 120
static int saved_fireball_x[MAX_FRAMES];
static int saved_soundwave_x[MAX_FRAMES];
static int saved_enemy_x[MAX_FRAMES];
static double saved_player_hp[MAX_FRAMES];
static double saved_enemy_hp[MAX_FRAMES];
static int saved_frames = 0;

static bool console_open = false;
static char typed_text[40] = "";
static int typed_len = 0;
static char last_output[80] = "Type 'help' and press ENTER";

// game.c variables
extern struct Characters player;
extern struct Characters* current_enemy;
extern int fireball_x;
extern int soundwave_x;
extern int enemy_x;
extern int current_round;
extern int current_level;
extern bool player_turn;
extern int is_paused;

void Debug_Init(void) {
    show_hitboxes = true;
    is_frozen = false;
    step_forward = false;
    console_open = false;
    saved_frames = 0;
}

/*void Debug_StartTimer(int section) {
    if (section == 0 || section == 1) {
        timer_start[section] = CP_System_GetMillis();
    }
}

void Debug_EndTimer(int section) {
    if (section == 0 || section == 1) {
        timer_result_ms[section] = CP_System_GetMillis() - timer_start[section];
    }
}

*/

bool Debug_ShouldUpdateGame(void) {
    if (!debug_mode || !is_frozen) {
        return true;
    }
    if (step_forward) {
        step_forward = false;
        return true;
    }
    return false;
}

void Debug_RecordFrame(void) {
    if (!debug_mode || is_frozen || current_enemy == NULL) {
        return;
    }
    // Shift old frames left if buffer is full
    if (saved_frames == MAX_FRAMES) {
        for (int i = 0; i < MAX_FRAMES - 1; i++) {
            saved_fireball_x[i] = saved_fireball_x[i + 1];
            saved_soundwave_x[i] = saved_soundwave_x[i + 1];
            saved_enemy_x[i] = saved_enemy_x[i + 1];
            saved_player_hp[i] = saved_player_hp[i + 1];
            saved_enemy_hp[i] = saved_enemy_hp[i + 1];
        }
        saved_frames--;
    }

    saved_fireball_x[saved_frames] = fireball_x;
    saved_soundwave_x[saved_frames] = soundwave_x;
    saved_enemy_x[saved_frames] = enemy_x;
    saved_player_hp[saved_frames] = player.current_hp;
    saved_enemy_hp[saved_frames] = current_enemy->current_hp;
    saved_frames++;
}


static void Rewind_One_Frame(void) {
    if (saved_frames > 1 && current_enemy != NULL) {
        saved_frames--; // go back 1 slot
        int prev = saved_frames - 1;

        // put the saved values back into the game
        fireball_x = saved_fireball_x[prev];
        soundwave_x = saved_soundwave_x[prev];
        enemy_x = saved_enemy_x[prev];
        player.current_hp = saved_player_hp[prev];
        current_enemy->current_hp = saved_enemy_hp[prev];
    }
}

/*static void Run_Console_Command(const char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        strcpy_s(last_output, sizeof(last_output), "Commands: heal, kill, level 1, level 2, level 3, hitboxes");
    }
    else if (strcmp(cmd, "heal") == 0) {
        player.current_hp = player.max_hp;
        strcpy_s(last_output, sizeof(last_output), "Player healed to Max HP!");
    }
    else if (strcmp(cmd, "kill") == 0 && current_enemy != NULL) {
        current_enemy->current_hp = 0;
        strcpy_s(last_output, sizeof(last_output), "Enemy HP set to 0!");
    }
    else if (strcmp(cmd, "hitboxes") == 0) {
        show_hitboxes = !show_hitboxes;
        strcpy_s(last_output, sizeof(last_output), show_hitboxes ? "Hitboxes ON" : "Hitboxes OFF");
    }
    else if (strncmp(cmd, "level ", 6) == 0) {
        //int lvl = (cmd + 6);
        if (lvl >= 1 && lvl <= 3) {
            current_level = lvl;
            current_round = 1;
            Check_Current_Enemy(current_level);
            Audio_Play_Level(current_level);
            sprintf_s(last_output, sizeof(last_output), "Jumped to Level %d", lvl);
        }
        else {
            strcpy_s(last_output, sizeof(last_output), "Use level 1, 2, or 3");
        }
    }
    else {
        sprintf_s(last_output, sizeof(last_output), "Unknown command: %s", cmd);
    }
}
*/

void Debug_Update(void) {
    // press D to toggle debug mode
    if (CP_Input_KeyTriggered(KEY_D)) {
        debug_mode = !debug_mode;
    }

    if (!debug_mode) {
        return;
    }

    // F1 toggles the drop-down command console
    if (CP_Input_KeyTriggered(KEY_F1)) {
        console_open = !console_open;
    }

    if (console_open) {
        return; // don't trigger hotkeys 
    }

    // F6: Freeze / Unfreeze frames
    if (CP_Input_KeyTriggered(KEY_F6)) {
        is_frozen = !is_frozen;
    }

    // right arrow = step forward 1 frame | left arrow = rewind 1 frame
    if (is_frozen && CP_Input_KeyTriggered(KEY_RIGHT)) {
        step_forward = true;
    }
    if (is_frozen && CP_Input_KeyTriggered(KEY_LEFT)) {
        Rewind_One_Frame();
    }

    // F7: toggle collision hitboxes
    if (CP_Input_KeyTriggered(KEY_F7)) {
        show_hitboxes = !show_hitboxes;
    }
}

// collision circles
void Debug_DrawCollisionCircles(void) {
    if (!debug_mode || !show_hitboxes) {
        return;
    }

    CP_Settings_NoFill();
    CP_Settings_StrokeWeight(3.0f);

    // player hitbox (green)
    CP_Settings_Stroke(CP_Color_Create(0, 255, 0, 255));
    CP_Graphics_DrawCircle((float)PLAYER_X_AXIS, (float)PLAYER_Y_AXIS, (float)PLAYER_WIDTH);

    // enemy hitbox (yellow)
    CP_Settings_Stroke(CP_Color_Create(255, 255, 0, 255));
    CP_Graphics_DrawCircle((float)enemy_x, (float)ENEMY_Y_AXIS, (float)ENEMY_WIDTH);

    // fireball hitbox (red) 
    if (player_turn) {
        CP_Settings_Stroke(CP_Color_Create(255, 60, 60, 255));
        CP_Graphics_DrawCircle((float)fireball_x, (float)PLAYER_Y_AXIS, 90.0f);
    }

    // soundwave hitbox (cyan) 
    if (!player_turn) {
        CP_Settings_Stroke(CP_Color_Create(0, 220, 255, 255));
        CP_Graphics_DrawCircle((float)soundwave_x, (float)ENEMY_Y_AXIS, 90.0f);
    }

    CP_Settings_NoStroke();
}

void Debug_Draw(void) {
    if (!debug_mode) {
        return;
    }

    char buf[128];
    float mx = CP_Input_GetMouseX();
    float my = CP_Input_GetMouseY();

    // debug panel
    CP_Settings_RectMode(CP_POSITION_CORNER);
    CP_Settings_NoStroke();
    CP_Settings_Fill(CP_Color_Create(15, 20, 30, 210));
    CP_Graphics_DrawRect(15.0f, 15.0f, 400.0f, 410.0f);

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_TextSize(18.0f);

    CP_Settings_Fill(CP_Color_Create(100, 255, 100, 255));
    CP_Font_DrawText("DEBUG MODE [D: Close | F1: Console]", 25.0f, 22.0f);

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    sprintf_s(buf, sizeof(buf), "FPS: %.0f | Mouse: (%.0f, %.0f)", CP_System_GetFrameRate(), mx, my);
    CP_Font_DrawText(buf, 25.0f, 48.0f);

  /*  // --- 2. CODE PROFILER BARS ---
    CP_Settings_Fill(CP_Color_Create(120, 220, 255, 255));
    CP_Font_DrawText("PROFILER (ms):", 25.0f, 78.0f);

    const char* labels[2] = { "Combat:", "UI/Pause:" };
    for (int i = 0; i < 2; i++) {
        float y = 102.0f + i * 24.0f;
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        sprintf_s(buf, sizeof(buf), "%s %.2f ms", labels[i], timer_result_ms[i]);
        CP_Font_DrawText(buf, 25.0f, y);

        float bar_w = timer_result_ms[i] * 15.0f;
        if (bar_w > 160.0f) bar_w = 160.0f;
        CP_Settings_Fill(timer_result_ms[i] > 8.0f ? CP_Color_Create(255, 80, 80, 255) : CP_Color_Create(100, 255, 100, 255));
        CP_Graphics_DrawRect(220.0f, y + 3.0f, bar_w, 12.0f);
    }
    */

    // ENTITY VARIABLE INSPECTOR & EDITOR
    CP_Settings_Fill(CP_Color_Create(120, 220, 255, 255));
    CP_Font_DrawText("ENTITY EDITOR (Click - / +):", 25.0f, 158.0f);

    // LEVEL
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    sprintf_s(buf, sizeof(buf), "Level: %d", current_level);
    CP_Font_DrawText(buf, 25.0f, 185.0f);
    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 270.0f, 193.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 305.0f, 193.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 193.0f);
    CP_Font_DrawText("+", 305.0f, 193.0f);
    if (IsAreaClicked(270.0f, 193.0f, 26.0f, 20.0f, mx, my) && current_level > 1) {
        current_level--;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }
    if (IsAreaClicked(305.0f, 193.0f, 26.0f, 20.0f, mx, my) && current_level < 3) {
        current_level++;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }

    // PLAYER HP
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Player HP: %.0f / %.0f", player.current_hp, player.max_hp);
    CP_Font_DrawText(buf, 25.0f, 213.0f);
    DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 221.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 221.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 221.0f);
    CP_Font_DrawText("+", 305.0f, 221.0f);
    if (IsAreaClicked(270.0f, 221.0f, 26.0f, 20.0f, mx, my)) {
        player.current_hp -= 200.0;
        if (player.current_hp < 0.0) player.current_hp = 0.0;
    }
    if (IsAreaClicked(305.0f, 221.0f, 26.0f, 20.0f, mx, my)) {
        player.current_hp += 200.0;
        if (player.current_hp > player.max_hp) player.current_hp = player.max_hp;
    }

    // PLAYER ATTACK 
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Player Atk: %.0f", player.attack);
    CP_Font_DrawText(buf, 25.0f, 241.0f);
    DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 249.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 249.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 249.0f);
    CP_Font_DrawText("+", 305.0f, 249.0f);
    if (IsAreaClicked(270.0f, 249.0f, 26.0f, 20.0f, mx, my) && player.attack > 50.0) {
        player.attack -= 50.0;
    }
    if (IsAreaClicked(305.0f, 249.0f, 26.0f, 20.0f, mx, my)) {
        player.attack += 50.0;
    }

    // ENEMY HP
    if (current_enemy != NULL) {
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
        sprintf_s(buf, sizeof(buf), "Enemy HP: %.0f / %.0f", current_enemy->current_hp, current_enemy->max_hp);
        CP_Font_DrawText(buf, 25.0f, 269.0f);
        DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 277.0f, 26.0f, 20.0f, 4.0f);
        DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 277.0f, 26.0f, 20.0f, 4.0f);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("-", 270.0f, 277.0f);
        CP_Font_DrawText("+", 305.0f, 277.0f);
        if (IsAreaClicked(270.0f, 277.0f, 26.0f, 20.0f, mx, my)) {
            current_enemy->current_hp -= 500.0;
            if (current_enemy->current_hp < 0.0) current_enemy->current_hp = 0.0;
        }
        if (IsAreaClicked(305.0f, 277.0f, 26.0f, 20.0f, mx, my)) {
            current_enemy->current_hp += 500.0;
            if (current_enemy->current_hp > current_enemy->max_hp) current_enemy->current_hp = current_enemy->max_hp;
        }
    }

    // FRAME & HITBOX
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
    sprintf_s(buf, sizeof(buf), "[F6] Freeze: %s", is_frozen ? "ON" : "OFF");
    CP_Font_DrawText(buf, 25.0f, 310.0f);

    CP_Font_DrawText("[Left / Right] Rewind & Step Frames", 25.0f, 336.0f);

    sprintf_s(buf, sizeof(buf), "[F7] Hitboxes: %s", show_hitboxes ? "ON" : "OFF");
    CP_Font_DrawText(buf, 25.0f, 362.0f);

    sprintf_s(buf, sizeof(buf), "Fireball X: %d | Wave X: %d", fireball_x, soundwave_x);
    CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
    CP_Font_DrawText(buf, 25.0f, 388.0f);

    // DROP DOWN DEBUG CONSOLE (top left)
    if (console_open) {
        CP_Settings_RectMode(CP_POSITION_CORNER);
        CP_Settings_Fill(CP_Color_Create(10, 12, 18, 240));
        CP_Graphics_DrawRect(0.0f, 0.0f, 1600.0f, 110.0f);

        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
        CP_Settings_TextSize(20.0f);

        CP_Settings_Fill(CP_Color_Create(100, 255, 100, 255));
        CP_Font_DrawText(last_output, 25.0f, 15.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 120, 255));
        sprintf_s(buf, sizeof(buf), "> %s_", typed_text);
        CP_Font_DrawText(buf, 25.0f, 55.0f);
    }
}
