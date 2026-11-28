#include "cprocessing.h"
#include "constants.h"
#include <stdbool.h>

static int frame = 0;
static float timer = 0.0f;
const float FRAME_DURATION = 0.50f;   // seconds per frame
const int FRAME_COUNT = 3;

static int skill_animation_frame_count = 0;
static bool is_spaceship_animation_active = false;
static bool is_meteorite_animation_active = false;
static bool is_bubble_animation_active = false;
static float skill_timer = 0.0f;
const float SKILL_FRAME_DURATION = 0.30f;
const int SKILL_ANIMATION_FRAMES = 3;

static float meteorite_movement_timer = 0.0f;
const float METEORITE_MOVEMENT_DURATION = 0.60f;

void Character_Animation(CP_Image sprite, bool is_player, bool top, float x) {

    int imageWidth = CP_Image_GetWidth(sprite);
    int imageHeight = CP_Image_GetHeight(sprite);

    timer += CP_System_GetDt();
    if (timer >= FRAME_DURATION) {
        timer -= FRAME_DURATION;
        frame = (frame + 1) % FRAME_COUNT;
    }

    float frameWidth = imageWidth / (float)FRAME_COUNT;

    CP_Image_DrawSubImage(
        sprite,
        x,
        (is_player) ? PLAYER_Y_AXIS : ENEMY_Y_AXIS,   // where to draw
        (is_player) ? 180.0f : 220.0f,
        (is_player) ? 220.0f : 150.0f,                       // size on screen
        frame * frameWidth, 
        ((top) ? 0 : imageHeight) / 2.0f,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight / ((top) ? 2.0f : 1.0f),                    // bottom edge (v1)
        255
    );
}

void Spaceship_Animation(CP_Image sprite) {

    if (!is_spaceship_animation_active) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)FRAME_COUNT;

    CP_Image_DrawSubImage(
        sprite,
        ENEMY_X_AXIS,
        ENEMY_Y_AXIS - 150,   // where to draw
        380,
        470,                       // size on screen
        frame * frameWidth,
        0,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight,                    // bottom edge (v1)
        225
    );

    skill_timer += CP_System_GetDt();
    if (skill_timer >= SKILL_FRAME_DURATION) {
        skill_timer -= SKILL_FRAME_DURATION;
    skill_animation_frame_count++;

    if (skill_animation_frame_count > SKILL_ANIMATION_FRAMES) {
        is_spaceship_animation_active = false;
        skill_animation_frame_count = 0;
        frame = 0;
            skill_timer = 0.0f;
    } else {
            frame = skill_animation_frame_count % FRAME_COUNT;
        }
    }
}

void Trigger_Spaceship_Animation(void) {
    if (!is_spaceship_animation_active) {
        is_spaceship_animation_active = true;
        skill_animation_frame_count = 0;
        frame = 0;
    }
}

void Meteorite_Animation(CP_Image sprite) {

    if (!is_meteorite_animation_active) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)FRAME_COUNT;
    float movementProgress = meteorite_movement_timer / METEORITE_MOVEMENT_DURATION;
    if (movementProgress > 1.0f) movementProgress = 1.0f;

    float startX = PLAYER_X_AXIS;
    float startY = PLAYER_Y_AXIS;
    float endX = ENEMY_X_AXIS;
    float endY = ENEMY_Y_AXIS;

    float currentX = startX + (endX - startX) * movementProgress;
    float currentY = startY + (endY - startY) * movementProgress;

    CP_Image_DrawSubImage(
        sprite,
        currentX,
        currentY,   // where to draw (interpolated position)
        200,
        150,                       // size on screen
        frame * frameWidth,
        0,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight,                    // bottom edge (v1)
        225
    );

    meteorite_movement_timer += CP_System_GetDt();
    skill_timer += CP_System_GetDt();
    if (skill_timer >= SKILL_FRAME_DURATION) {
        skill_timer -= SKILL_FRAME_DURATION;
        skill_animation_frame_count++;

        if (skill_animation_frame_count > SKILL_ANIMATION_FRAMES) {
            is_meteorite_animation_active = false;
            skill_animation_frame_count = 0;
            frame = 0;
            skill_timer = 0.0f;
            meteorite_movement_timer = 0.0f;
        }
        else {
            frame = skill_animation_frame_count % FRAME_COUNT;
        }
    }
}

void Trigger_Meteorite_Animation(void) {
    if (!is_meteorite_animation_active) {
        is_meteorite_animation_active = true;
        skill_animation_frame_count = 0;
        frame = 0;
        meteorite_movement_timer = 0.0f;
    }
}

void Bubble_Animation(CP_Image sprite) {

    if (!is_bubble_animation_active) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)FRAME_COUNT;

    CP_Image_DrawSubImage(
        sprite,
        PLAYER_X_AXIS,
        PLAYER_Y_AXIS,   // where to draw
        300,
        300,                       // size on screen
        frame * frameWidth,
        0,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight,                    // bottom edge (v1)
        225
    );

    skill_timer += CP_System_GetDt();
    if (skill_timer >= SKILL_FRAME_DURATION) {
        skill_timer -= SKILL_FRAME_DURATION;
        frame = (frame + 1) % FRAME_COUNT;
    }
}

void Trigger_Bubble_Animation(void) {
    if (!is_bubble_animation_active) {
        is_bubble_animation_active = true;
        skill_animation_frame_count = 0;
        frame = 0;
        skill_timer = 0.0f;
    }
}

void End_Bubble_Animation(void) {
    is_bubble_animation_active = false;
    skill_animation_frame_count = 0;
    frame = 0;
    skill_timer = 0.0f;
}

bool Is_Bubble_Animation_Active(void) {
    return is_bubble_animation_active;
}