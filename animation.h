#pragma once
#include <stdbool.h>

void CharacterAnimation(CP_Image sprite, bool isPlayer, bool top, float x);

void SpaceshipAnimation(CP_Image sprite);
void MeteoriteAnimation(CP_Image sprite);
void BubbleAnimation(CP_Image sprite);
void TriggerBubbleAnimation(void);
void EndBubbleAnimation(void);
bool IsBubbleAnimationActive(void);