#pragma once
#include "GameObject.h"

class StaticObject : public CGameObject
{
	int spriteID;

public:
	void SetSpriteID(int spriteID) { this->spriteID = spriteID; }
	void Update(DWORD dt);
	void Render();
};
