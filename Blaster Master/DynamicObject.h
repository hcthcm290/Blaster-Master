#pragma once
#include "GameObject.h"
#include "Animator.h"

class DynamicObject : public CGameObject
{
protected:
	float vx;
	float vy;

	Animator animator;
};