#pragma once
#include "Animator_Jumper.h"
#include "Debug.h"

void Animator_Jumper::Draw(int animationID, float x, float y, bool flipX) {
	if (animationID != curAnimationID)
	{
		curFrame = 0;
		curAnimationID = animationID;
	}
	curFrame = animationSet[animationID]->Render(curFrame, x, y, flipX);
}