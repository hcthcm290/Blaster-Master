#pragma once
#include "GameObject.h"

class StaticObject : public CGameObject
{
	int spriteID;

public:
	void SetSpriteID(int spriteID) { this->spriteID = spriteID; }
	D3DRECT GetCollisionBox() { return D3DRECT(); }
	void Update(DWORD dt);
	void Render();
};
