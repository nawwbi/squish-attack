#include "cprocessing.h"
#include "constants.h"
#include <stdbool.h>

static int frame = 0;
static float timer = 0.0f;
const float frameDuration = 0.50f;   // seconds per frame
const int frameCount = 3;

static int skillAnimationFrameCount = 0;
static bool isSpaceshipAnimationActive = false;
static bool isMeteoriteAnimationActive = false;
static bool isBubbleAnimationActive = false;
static float skillTimer = 0.0f;
const float skillFrameDuration = 0.30f;
const int SKILL_ANIMATION_FRAMES = 3;

static float meteoriteMovementTimer = 0.0f;
const float meteoriteMovementDuration = 0.60f;

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
        (isPlayer) ? 180.0f : 220.0f,
        (isPlayer) ? 220.0f : 150.0f,                       // size on screen
        frame * frameWidth, 
        ((top) ? 0 : imageHeight) / 2.0f,          // top-left of the source frame (u0, v0)
        (frame + 1) * frameWidth,       // right edge of the source frame (u1)
        imageHeight / ((top) ? 2.0f : 1.0f),                    // bottom edge (v1)
        255
    );
}

void SpaceshipAnimation(CP_Image sprite) {

    if (!isSpaceshipAnimationActive) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

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

    skillTimer += CP_System_GetDt();
    if (skillTimer >= frameDuration) {
        skillTimer -= frameDuration;
    skillAnimationFrameCount++;

    if (skillAnimationFrameCount > SKILL_ANIMATION_FRAMES) {
        isSpaceshipAnimationActive = false;
        skillAnimationFrameCount = 0;
        frame = 0;
            skillTimer = 0.0f;
    } else {
            frame = skillAnimationFrameCount % frameCount;
        }
    }
}

void TriggerSpaceshipAnimation(void) {
    if (!isSpaceshipAnimationActive) {
        isSpaceshipAnimationActive = true;
        skillAnimationFrameCount = 0;
        frame = 0;
    }
}

void MeteoriteAnimation(CP_Image sprite) {

    if (!isMeteoriteAnimationActive) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)frameCount;
    float movementProgress = meteoriteMovementTimer / meteoriteMovementDuration;
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

    meteoriteMovementTimer += CP_System_GetDt();
    skillTimer += CP_System_GetDt();
    if (skillTimer >= skillFrameDuration) {
        skillTimer -= skillFrameDuration;
        skillAnimationFrameCount++;

        if (skillAnimationFrameCount > SKILL_ANIMATION_FRAMES) {
            isMeteoriteAnimationActive = false;
            skillAnimationFrameCount = 0;
            frame = 0;
            skillTimer = 0.0f;
            meteoriteMovementTimer = 0.0f;
        }
        else {
            frame = skillAnimationFrameCount % frameCount;
        }
    }
}

void TriggerMeteoriteAnimation(void) {
    if (!isMeteoriteAnimationActive) {
        isMeteoriteAnimationActive = true;
        skillAnimationFrameCount = 0;
        frame = 0;
        meteoriteMovementTimer = 0.0f;
    }
}

void BubbleAnimation(CP_Image sprite) {

    if (!isBubbleAnimationActive) {
        return;
    }

    float imageWidth = (float)CP_Image_GetWidth(sprite);
    float imageHeight = (float)CP_Image_GetHeight(sprite);

    float frameWidth = imageWidth / (float)frameCount;

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

    skillTimer += CP_System_GetDt();
    if (skillTimer >= frameDuration) {
        skillTimer -= frameDuration;
        frame = (frame + 1) % frameCount;
    }
}

void TriggerBubbleAnimation(void) {
    if (!isBubbleAnimationActive) {
        isBubbleAnimationActive = true;
        skillAnimationFrameCount = 0;
        frame = 0;
        skillTimer = 0.0f;
    }
}

void EndBubbleAnimation(void) {
    isBubbleAnimationActive = false;
    skillAnimationFrameCount = 0;
    frame = 0;
    skillTimer = 0.0f;
}

bool IsBubbleAnimationActive(void) {
    return isBubbleAnimationActive;
}