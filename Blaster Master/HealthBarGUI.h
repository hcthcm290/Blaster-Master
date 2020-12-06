#pragma once
#include "GameObject.h"

class HealthBarGUI : public CGameObject
{
private:
	int maxHP;
	int currentHP;

	D3DXVECTOR2 offset; // offset position of healthbar calculate from the center of the screen

public:
	HealthBarGUI();
	virtual FRECT GetCollisionBox() { return FRECT(); }
	virtual void Update(float dt);
	virtual void Render();
};