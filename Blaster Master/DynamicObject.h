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
	int maxHP;

public:
	int HP = 0;
	int inv;
	DWORD last_blink;
	DWORD startTakeDamage;
	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(255,255,75,75),
	D3DCOLOR_ARGB(128,255,255,255)
	};
	DynamicObject();
	virtual void TakeDamage(int dmg);
	virtual int GetCurrentHP() { return this->HP; }
	virtual int GetMaxHP() { return this->maxHP; }
	FRECT GetCollisionBox() { return FRECT(x - 1, y - 1, x + 1, y + 1); }
	D3DVECTOR GetVelocity();
	void SetVelocity(float vx, float vy) { this->vx = vx; this->vy = vy; }
};