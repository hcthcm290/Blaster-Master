#pragma once
#include "Sprites.h"

class Animator
{
protected:
	int curFrame = 0;
	int curAnimationID = -1;

public:
	virtual void Draw(int animationID, float x, float y, bool flipX);
};