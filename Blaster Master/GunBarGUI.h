#pragma once
#include "GameObject.h"

class GunBarGUI : public CGameObject
{
private:
	int gunPower;

	D3DXVECTOR2 offset; // offset position of healthbar calculate from the center of the screen

public:
	GunBarGUI();
	virtual FRECT GetCollisionBox() { return FRECT(); }
	virtual void Update(float dt);
	virtual void Render();
};