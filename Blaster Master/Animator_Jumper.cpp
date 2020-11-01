#pragma once
#include "Animator_Jumper.h"

void Animator_Jumper::Draw(int animationID, float x, float y, bool flipX) {
	if (animationID != curAnimationID) curFrame = 0;
	curFrame = CAnimations::GetInstance()->Get(animationID)->Render(curFrame, x, y, flipX);
}