#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "debug.h"
#include "constants.h" // struct characters
#include <stdio.h>
#include <stdbool.h>

extern int current_round;
extern int current_level;
extern int is_paused;
extern struct Characters player;
extern struct Characters* current_enemy;
extern float fireball_x;
extern float boss_current_x;
extern bool player_turn;

static bool show_debug_overlay = false;

void Debug_Init(void) {
	show_debug_overlay = false;
}

void Debug_Update(void) {
	if (CP_Input_KeyTriggered(KEY_D)) {
		show_debug_overlay = !show_debug_overlay;
	}

	// cheat keys
	if (show_debug_overlay) {
		// force skip to next round
		if (CP_Input_KeyTriggered(KEY_Q)) {
			current_round++; // but not updating
		}
		// force previous round 
		if (CP_Input_KeyTriggered(KEY_W)) {
			current_round--;
		}
		// instant heal current hp
		if (CP_Input_KeyTriggered(KEY_H)) {
			player.current_hp = player.max_hp;
		}

	}
}