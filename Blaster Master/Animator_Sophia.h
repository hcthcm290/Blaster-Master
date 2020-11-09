#pragma once
#include "Animator.h"
class Animator_Sophia : public Animator
{
public:
	bool isResetFrame = false;
	int wheel;
	Animator_Sophia();
	void Draw(int animationID, float x, float y, bool flipX);
};

