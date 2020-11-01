#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "DInput.h"

class DynamicObject : public CGameObject
{
protected:
	float vx = 0;
	float vy = 0;

	Animator animator;

public:
	FRECT GetCollisionBox() { return FRECT();}
	D3DVECTOR GetVelocity();
	void SetVelocity(float vx, float vy) { this->vx = vx; this->vy = vy; }
};