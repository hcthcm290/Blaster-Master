#pragma once
#include "Animator.h"

class Animator_Jason: public Animator 
{
private:
	void Draw(int animationID, float x, float y, bool flipX);
public:
	Animator_Jason();
};