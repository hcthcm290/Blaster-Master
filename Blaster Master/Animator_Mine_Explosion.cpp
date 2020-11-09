#include "Animator_Mine_Explosion.h"
#include "Debug.h"

void Animator_Mine_Explosion::Draw(int animationID, float x, float y, bool flipX)
{
	if (doneAnimated) return;

	LPANIMATION anim = animationSet[animationID];

	if (anim == NULL)
	{
		DebugOut(L"animation id not found");
		return;
	}

	if (animationID != curAnimationID)
	{
		curFrame = -1;
	}

	curAnimationID = animationID;

	int tempFrame = anim->Render(curFrame, x, y, flipX);

	if (curFrame == 1)
	{
		started = true;
	}

	if (tempFrame == 0 && started) // when animation is still not finish
	{
		doneAnimated = true;
	}
	else
	{
		curFrame = tempFrame;
	}

}
