#include "cprocessing.h"
#include "constants.h"
#include <stdbool.h>

static int frame = 0;
static float timer = 0.0f;
const float frameDuration = 0.30f;   // seconds per frame
const int frameCount = 3;

void Animation(CP_Image sprite, bool isPlayer, bool top, float x) {
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