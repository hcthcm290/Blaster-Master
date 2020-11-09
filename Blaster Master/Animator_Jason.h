#pragma once
#include "Animator.h"

class Animator_Jason: public Animator 
{
public:
	Animator_Jason();
	void Draw(int animationID, float x, float y, bool flipX);
};