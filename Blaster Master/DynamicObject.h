#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "DInput.h"
#include "StateConstant.h"

class DynamicObject : public CGameObject
{
protected:
	float vx = 0;
	float vy = 0;

	Animator* animator = new Animator();

public:
	FRECT GetCollisionBox() { return FRECT(x - 1, y - 1, x + 1, y + 1); }
	D3DVECTOR GetVelocity();
	void SetVelocity(float vx, float vy) { this->vx = vx; this->vy = vy; }
};