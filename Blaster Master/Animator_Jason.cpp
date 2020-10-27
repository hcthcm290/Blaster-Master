#pragma once
#include "Animator_Jason.h"

void Animator_Jason::Draw(int animationID, float x, float y, bool flipX) {
	curFrame = CAnimations::GetInstance()->Get(animationID)->Render(curFrame, x, y, flipX);
}