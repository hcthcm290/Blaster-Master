#include "Animator.h"
#include "Debug.h"
#include <string>
//#include <atlconv.h>

void Animator::Draw(int animationID, float x, float y, bool flipX)
{
	LPANIMATION anim = animationSet[animationID];

	if (anim == NULL)
	{
		DebugOut(L"animation id not found");
		return;
	}

	if (animationID != curAnimationID)
	{
		curFrame = 0;
	}

	curAnimationID = animationID;

	curFrame = anim->Render(curFrame, x, y, flipX);
}

void Animator::Draw(int animationID, float x, float y, bool flipX, float rotation)
{
	LPANIMATION anim = animationSet[animationID];

	if (anim == NULL)
	{
		DebugOut(L"animation id not found");
		return;
	}

	if (animationID != curAnimationID)
	{
		curFrame = 0;
	}

	curAnimationID = animationID;

	curFrame = anim->Render(curFrame, x, y, flipX, rotation);
}

void Animator::AddAnimation(int id)
{
	animationSet[id] = new CAnimation(*CAnimations::GetInstance()->Get(id));
}
