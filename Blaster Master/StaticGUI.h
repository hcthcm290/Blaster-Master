#pragma once
#include "GameObject.h"

class StaticGUI : public CGameObject
{
private:
	D3DXVECTOR2 offset; // offset position of GUI calculate from the center of the screen
	int spriteID;

public:
	StaticGUI(int spriteID, D3DXVECTOR2 offset);
	virtual FRECT GetCollisionBox() { return FRECT(); }
	virtual void Update(float dt);
	virtual void Render();
};