#pragma once
#include "Animator_Jumper.h"

void Animator_Jumper::Draw(int animationID, float x, float y, bool flipX) {
	curFrame = CAnimations::GetInstance()->Get(animationID)->Render(curFrame, x, y, flipX);
	//if (animationID != curAnimationID) curFrame = 0;
}