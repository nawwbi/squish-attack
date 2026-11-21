#include "cprocessing.h"
#include "constants.h"
#include <stdbool.h>

static int frame = 0;
static float timer = 0.0f;
const float frameDuration = 0.50f;   // seconds per frame
const int frameCount = 3;

// Skill animation state
static int skillAnimationFrameCount = 0;
static bool isSkillAnimationActive = false;
static float skillTimer = 0.0f;
const float skillFrameDuration = 0.30f;
const int SKILL_ANIMATION_FRAMES = 3;  // Display for 3 frames

void CharacterAnimation(CP_Image sprite, bool isPlayer, bool top, float x) {
    //int width = CP_System_GetWindowWidth();
    //int height = CP_System_GetWindowHeight();

    int imageWidth = CP_Image_GetWidth(sprite);
    int imageHeight = CP_Image_GetHeight(sprite);

    timer += CP_System_GetDt();
    if (timer >= frameDuration) {
        timer -= frameDuration;
        frame = (frame + 1) % frameCount;
    }

    float frameWidth = imageWidth / (float)frameCount;

    CP_Image_DrawSubImage(
        sprite,
        x,
        (isPlayer) ? PLAYER_Y_AXIS : ENEMY_Y_AXIS,   // where to draw
        (isPlayer) ? 180 : 220,
        (isPlayer) ? 220 : 150,                       // size on screen
        frame * frameWidth, 
        ((top) ? 0 : imageHeight) / 2,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight / ((top) ? 2 : 1),                    // bottom edge (v1)
        255
    );
}

void SkillAnimation(CP_Image sprite) {

    if (!isSkillAnimationActive) {
        return;  // Don't draw if animation isn't active
    }

    int imageWidth = CP_Image_GetWidth(sprite);
    int imageHeight = CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)frameCount;

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

    // Update timer and advance frames
    skillTimer += CP_System_GetDt();
    if (skillTimer >= frameDuration) {
        skillTimer -= frameDuration;
    skillAnimationFrameCount++;

    if (skillAnimationFrameCount > SKILL_ANIMATION_FRAMES) {
        isSkillAnimationActive = false;
        skillAnimationFrameCount = 0;
        frame = 0;
            skillTimer = 0.0f;
    } else {
            frame = skillAnimationFrameCount % frameCount;
        }
    }
}

void TriggerSkillAnimation(void) {
    if (!isSkillAnimationActive) {
        isSkillAnimationActive = true;
        skillAnimationFrameCount = 0;
        frame = 0;
    }
}