#include "Animator.h"
#include "Debug.h"
#include <string>
//#include <atlconv.h>

void Animator::Draw(int animationID, float x, float y, bool flipX)
{
	LPANIMATION anim = CAnimations::GetInstance()->Get(animationID);

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
