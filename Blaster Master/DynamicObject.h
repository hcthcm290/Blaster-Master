#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "DInput.h"
#include "StateConstant.h"

class DynamicObject : public CGameObject
{
protected:
	float vx;
	float vy;

	Animator* animator = new Animator();
};