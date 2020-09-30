#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "DInput.h"

class DynamicObject : public CGameObject
{
protected:
	float vx;
	float vy;

	Animator animator;
};