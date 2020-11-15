#pragma once
#include "Animator_Sophia.h"
#include "Debug.h"

Animator_Sophia::Animator_Sophia()
{
	//Idle, gun 0
	AddAnimation(29801);
	AddAnimation(29802);
	AddAnimation(29803);
	AddAnimation(29804);
	//Moving, gun 0
	AddAnimation(29805);
	AddAnimation(29806);
	AddAnimation(29807);
	AddAnimation(29808);
	//Jumping, gun 0
	AddAnimation(29811);
	AddAnimation(29812);
	AddAnimation(29813);
	AddAnimation(29814);
	//Falling, gun 0
	AddAnimation(29815);
	AddAnimation(29816);
	AddAnimation(29817);
	AddAnimation(29818);
	//Moving, gun 45
	AddAnimation(29821);
	AddAnimation(29822);
	AddAnimation(29823);
	AddAnimation(29824);
	//Moving, gun 90
	AddAnimation(29825);
	AddAnimation(29826);
	AddAnimation(29827);
	AddAnimation(29828);
	//Jumping, gun 90
	AddAnimation(29831);
	AddAnimation(29832);
	AddAnimation(29833);
	AddAnimation(29834);
	//Falling, gun 90
	AddAnimation(29835);
	AddAnimation(29836);
	AddAnimation(29837);
	AddAnimation(29838);
	//Moving, gun turn
	AddAnimation(29841);
	AddAnimation(29842);
	AddAnimation(29843);
	AddAnimation(29844);
	//jumping, gun turn
	AddAnimation(29845);
	AddAnimation(29846);
	AddAnimation(29847);
	AddAnimation(29848);
	//idle, gun 90
	AddAnimation(29851);
	AddAnimation(29852);
	AddAnimation(29853);
	AddAnimation(29854);
	//jumping, gun 45
	AddAnimation(29855);
	AddAnimation(29856);
	AddAnimation(29857);
	AddAnimation(29858);
	//falling, gun 45
	AddAnimation(29861);
	AddAnimation(29862);
	AddAnimation(29863);
	AddAnimation(29864);
	//falling, gun turn
	AddAnimation(29865);
	AddAnimation(29866);
	AddAnimation(29867);
	AddAnimation(29868);
	//shift/sleep
	AddAnimation(29871);

	AddAnimation(29875);

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