#pragma once
#include "Animator_Floater.h"
#include "Debug.h"

void Animator_Floater::Draw(int animationID, float x, float y, bool flipX) {
	if (animationID != curAnimationID)
	{
		curFrame = 0;
		curAnimationID = animationID;
	}
	curFrame = animationSet[animationID]->Render(curFrame, x, y, flipX);
}