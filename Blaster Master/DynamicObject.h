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
	D3DRECT GetCollisionBox() { return D3DRECT();}
	D3DVECTOR GetVelocity();
};