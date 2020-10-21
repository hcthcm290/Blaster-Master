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

public:
	D3DRECT GetCollisionBox() { return D3DRECT();}
	D3DVECTOR GetVelocity();
};