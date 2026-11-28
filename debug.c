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
static bool frame_forward = false;
static bool frame_rewind = false;

#define MAX_FRAMES 120
static int saved_fireball_x[MAX_FRAMES];
static int saved_soundwave_x[MAX_FRAMES];
static int saved_enemy_x[MAX_FRAMES];
static float saved_player_hp[MAX_FRAMES];
static float saved_enemy_hp[MAX_FRAMES];
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
    frame_forward = false;
    console_open = false;
    saved_frames = 0;
}


bool Debug_ShouldUpdateGame(void) {
    if (!debug_mode || !is_frozen) {
        return true;
    }
    if (frame_forward) {
        frame_forward = false;
        return true;
    }
    return false;
}

void Debug_RecordFrame(void) {
    if (!debug_mode || is_frozen || current_enemy == NULL) return;

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
        saved_frames--;
        int prev = saved_frames - 1;

        fireball_x = saved_fireball_x[prev];
        soundwave_x = saved_soundwave_x[prev];
        enemy_x = saved_enemy_x[prev];
        player.current_hp = saved_player_hp[prev];
        current_enemy->current_hp = saved_enemy_hp[prev];
    }
}

void Debug_Update(void) {
    // press D to toggle debug mode
    if (CP_Input_KeyTriggered(KEY_D)) {
        debug_mode = !debug_mode;
    }

    if (!debug_mode) {
        return;
    }

    // toggles the drop down debug info command console
    if (CP_Input_KeyTriggered(KEY_Q)) {
        console_open = !console_open;
    }

    if (console_open) {
        return; // don't trigger hotkeys 
    } 

    // key F = Freeze / Unfreeze frames
    if (CP_Input_KeyTriggered(KEY_F)) {
        is_frozen = !is_frozen;
    }

    // key S = step forward 1 frame, key A = rewind 1 frame
    if (is_frozen && CP_Input_KeyTriggered(KEY_S)) {
        frame_forward = true;
    }
    if (is_frozen && CP_Input_KeyTriggered(KEY_A)) {
        Rewind_One_Frame();
    }

    // F7: toggle collision hitboxes
    if (CP_Input_KeyTriggered(KEY_C)) {
        show_hitboxes = !show_hitboxes;
    }
}

// collision circles
void Debug_DrawCollisionCircles(void) {
    char coord_buf[32];

    if (!debug_mode || !show_hitboxes) {
        return;
    }

    CP_Settings_NoFill();
    CP_Settings_StrokeWeight(3.0f);

    // player hitbox (green)
    CP_Settings_Stroke(CP_Color_Create(0, 255, 0, 255));
    CP_Graphics_DrawCircle((float)PLAYER_X_AXIS, (float)PLAYER_Y_AXIS, (float)PLAYER_WIDTH);

    // display player coordinates above player
    sprintf_s(coord_buf, sizeof(coord_buf), "Player: %.0f, %.0f", (float)PLAYER_X_AXIS, (float)PLAYER_Y_AXIS);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(24);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(coord_buf, (float)PLAYER_X_AXIS, (float)PLAYER_Y_AXIS + 110.0f);
    CP_Settings_NoFill();

    // enemy hitbox (yellow)
    CP_Settings_Stroke(CP_Color_Create(255, 255, 0, 255));
    CP_Graphics_DrawCircle((float)enemy_x, (float)ENEMY_Y_AXIS, (float)ENEMY_WIDTH);

    // display enemy coordinates above enemy
    sprintf_s(coord_buf, sizeof(coord_buf), "Enemy: %.0f, %.0f", (float)enemy_x, (float)ENEMY_Y_AXIS);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(24);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(coord_buf, (float)enemy_x, (float)ENEMY_Y_AXIS + 110.0f);
    CP_Settings_NoFill();

    // fireball hitbox (red) 
    if (player_turn) {
        CP_Settings_Stroke(CP_Color_Create(255, 60, 60, 255));
        CP_Graphics_DrawCircle((float)fireball_x, (float)PLAYER_Y_AXIS, 90.0f);

        // display x coordinate above the fireball
        sprintf_s(coord_buf, sizeof(coord_buf), "X: %d", fireball_x);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(24);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText(coord_buf, (float)fireball_x, (float)PLAYER_Y_AXIS - 65.0f);
    }

    // soundwave hitbox (cyan) 
    if (!player_turn) {
        CP_Settings_Stroke(CP_Color_Create(0, 220, 255, 255));
        CP_Graphics_DrawCircle((float)soundwave_x, (float)ENEMY_Y_AXIS, 90.0f);

        // display x coordinate above the soundwave
        sprintf_s(coord_buf, sizeof(coord_buf), "X: %d", soundwave_x);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Settings_TextSize(24);
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText(coord_buf, (float)soundwave_x, (float)ENEMY_Y_AXIS - 65.0f);
    }

    CP_Settings_NoStroke();
}

void Debug_Draw(void) {
    if (!debug_mode) {
        return;
    }

    char buf[128];

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
    sprintf_s(buf, sizeof(buf), "FPS: %.0f | Mouse: (%.0f, %.0f)", CP_System_GetFrameRate(), CP_Input_GetMouseX(), CP_Input_GetMouseY());
    CP_Font_DrawText(buf, 25.0f, 48.0f);

    // cheat keys press q text
    CP_Settings_Fill(CP_Color_Create(255, 80, 80, 255)); 
    CP_Font_DrawText("Cheat Keys (Press Q)", 25.0f, 102.0f);

    // ENTITY VARIABLE INSPECTOR & EDITOR
    CP_Settings_Fill(CP_Color_Create(120, 220, 255, 255));
    CP_Font_DrawText("ENTITY EDITOR (Click - / +):", 25.0f, 128.0f); // dif 27 y to level: %d

    // LEVEL
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Level: %d", current_level);
    CP_Font_DrawText(buf, 25.0f, 155.0f); // diff 28 y from one entity editor to another

    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 270.0f, 163.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 305.0f, 163.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 163.0f);
    CP_Font_DrawText("+", 305.0f, 163.0f);

    // previous level button
    if ((IsAreaClicked(270.0f, 163.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_J)) && current_level > 1 ) {
        current_level--;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }

    // next round button
    if ((IsAreaClicked(305.0f, 163.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_L)) && current_level < 3 ) {
        current_level++;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }

    // ROUND
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Round: %d", current_round);
    CP_Font_DrawText(buf, 25.0f, 183.0f); // diff 28 y from one entity editor to another

    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 270.0f, 191.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 130, 220, 255), 305.0f, 191.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 191.0f);
    CP_Font_DrawText("+", 305.0f, 191.0f);

    // previous round button
    if ((IsAreaClicked(270.0f, 191.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_E)) && current_round > 1) {
        current_round--;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }

    // next round button
    if ((IsAreaClicked(305.0f, 191.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_R)) && current_round < 10) {
        current_round++;
        Check_Current_Enemy(current_level);
        Audio_Play_Level(current_level);
    }

    // PLAYER 
    // player hp
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Player HP: %.0f / %.0f", player.current_hp, player.max_hp);
    CP_Font_DrawText(buf, 25.0f, 211.0f);

    DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 221.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 221.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 221.0f);
    CP_Font_DrawText("+", 305.0f, 221.0f);

    // decrease player current hp -200
    if (IsAreaClicked(270.0f, 221.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_G)) {
        player.current_hp -= 200.0;
        if (player.current_hp < 0.0) player.current_hp = 0.0;
    }

    // increase player current hp +200 
    if (IsAreaClicked(305.0f, 221.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_H)) {
        player.current_hp += 200.0;
        if (player.current_hp > player.max_hp) player.current_hp = player.max_hp;
    }

    // player attack
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    sprintf_s(buf, sizeof(buf), "Player Atk: %.0f", player.attack);
    CP_Font_DrawText(buf, 25.0f, 239.0f);

    DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 249.0f, 26.0f, 20.0f, 4.0f);
    DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 249.0f, 26.0f, 20.0f, 4.0f);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText("-", 270.0f, 249.0f);
    CP_Font_DrawText("+", 305.0f, 249.0f);

    // decrease player attack -50
    if ((IsAreaClicked(270.0f, 249.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_O)) && player.attack > 50.0 ) {
        player.attack -= 50.0;
    }

    // increase player attack +50
    if (IsAreaClicked(305.0f, 249.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_P)) {
        player.attack += 50.0;
    }

    // ENEMY
    // enemy hp
    if (current_enemy != NULL) {
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
        sprintf_s(buf, sizeof(buf), "Enemy HP: %.0f / %.0f", current_enemy->current_hp, current_enemy->max_hp);
        CP_Font_DrawText(buf, 25.0f, 267.0f);

        DrawRectangle(CP_Color_Create(200, 70, 70, 255), 270.0f, 277.0f, 26.0f, 20.0f, 4.0f);
        DrawRectangle(CP_Color_Create(70, 180, 90, 255), 305.0f, 277.0f, 26.0f, 20.0f, 4.0f);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Font_DrawText("-", 270.0f, 277.0f);
        CP_Font_DrawText("+", 305.0f, 277.0f);

        // decrease current enemy hp -500
        if (IsAreaClicked(270.0f, 277.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_N)) {
            current_enemy->current_hp -= 500.0;
            if (current_enemy->current_hp < 0.0) current_enemy->current_hp = 0.0;
        }

        // increase current enemy hp +500
        if (IsAreaClicked(305.0f, 277.0f, 26.0f, 20.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_M)) {
            current_enemy->current_hp += 500.0;
            if (current_enemy->current_hp > current_enemy->max_hp) current_enemy->current_hp = current_enemy->max_hp;
        }
    }

    // FRAME & HITBOX
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
    sprintf_s(buf, sizeof(buf), "[key F] Freeze: %s", is_frozen ? "ON" : "OFF");
    CP_Font_DrawText(buf, 25.0f, 310.0f);

    CP_Font_DrawText("[key A / key S] Rewind & Step Frames", 25.0f, 336.0f);

    sprintf_s(buf, sizeof(buf), "[key C] Hitboxes: %s", show_hitboxes ? "ON" : "OFF");
    CP_Font_DrawText(buf, 25.0f, 362.0f);

    sprintf_s(buf, sizeof(buf), "Fireball X: %d | Wave X: %d", fireball_x, soundwave_x);
    CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
    CP_Font_DrawText(buf, 25.0f, 388.0f);

    // drow down debug info (key q)
    if (console_open) {
        CP_Settings_RectMode(CP_POSITION_CORNER);
        CP_Settings_Fill(CP_Color_Create(10, 12, 18, 245));
        CP_Graphics_DrawRect(0.0f, 0.0f, 1600.0f, 280.0f); // console background height

        CP_Settings_TextSize(60);
        CP_Font_DrawText("Debug Info", 30.0f, 5.0f);

        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
        CP_Settings_TextSize(18.0f);

        // column 1
        CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
        CP_Font_DrawText("CHEAT KEYS", 30.0f, 20.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText("key D - debug mode on/off", 30.0f, 50.0f);
        CP_Font_DrawText("key F - freeze/unfreeze frames", 30.0f, 80.0f);
        CP_Font_DrawText("key A - rewind 1 frame", 30.0f, 110.0f);
        CP_Font_DrawText("key S - forward 1 frame", 30.0f, 140.0f);
        CP_Font_DrawText("key C - collision hitbox", 30.0f, 170.0f);
        CP_Font_DrawText("key Q - open drop down debug info", 30.0f, 200.0f);
        
        // column 2
        CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
        CP_Font_DrawText("LEVEL", 600.0f, 20.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText("key L - next level", 600.0f, 50.0f);
        CP_Font_DrawText("key J - previous level", 600.0f, 80.0f);

        CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
        CP_Font_DrawText("ROUND", 600.0f, 130.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText("key R - next round", 600.0f, 160.0f);
        CP_Font_DrawText("key E - previous round", 600.0f, 190.0f);

        // column 3
        CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
        CP_Font_DrawText("PLAYER", 1100.0f, 20.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText("key H - player current HP +200", 1100.0f, 50.0f);
        CP_Font_DrawText("key G - player current HP -200", 1100.0f, 80.0f);
        CP_Font_DrawText("key P - player attack + 50", 1100.0f, 110.0f);
        CP_Font_DrawText("key O - player attack -50", 1100.0f, 140.0f);

        CP_Settings_Fill(CP_Color_Create(255, 230, 100, 255));
        CP_Font_DrawText("ENEMIES", 1100.0f, 185.0f);

        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText("key M - enemy current HP +500", 1100.0f, 215.0f);
        CP_Font_DrawText("key N - enemy current HP -500", 1100.0f, 245.0f);
    }
}

void Debug_Exit() {

}