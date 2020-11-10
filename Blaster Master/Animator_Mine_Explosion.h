#pragma once
#include "Animator.h"

class Animator_Mine_Explosion : public Animator
{
private:
	bool started = false; // only started when currentFrame equals 1
	bool doneAnimated = false;

public:
	void Draw(int animationID, float x, float y, bool flipX);
	bool isDone() { return doneAnimated; }
};