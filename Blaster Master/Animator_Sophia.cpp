#pragma once
#include "Animator_Sophia.h"
#include "Debug.h"

Animator_Sophia::Animator_Sophia()
{
	this->isResetFrame = false;
	this->wheel = 1;
}
void Animator_Sophia::Draw(int animationID, float x, float y, bool flipX)
{
	LPANIMATION anim = animationSet[animationID];

	if (anim == NULL)
	{
		DebugOut(L"animation id not found");
		return;
	}

	if (this->isResetFrame)
	{
		curFrame = 0;
	}

	curAnimationID = animationID;

	curFrame = anim->Render(curFrame, x, y, flipX);
}