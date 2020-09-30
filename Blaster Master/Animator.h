#pragma once
#include "Sprites.h"

class Animator
{
private:
	int curFrame = 0;
	int curAnimationID = -1;

public:
	void Draw(int animationID, float x, float y, bool flipX);
};