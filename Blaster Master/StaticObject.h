#pragma once
#include "GameObject.h"

class StaticObject : public CGameObject
{
	int spriteID;

public:
	void SetSpriteID(int spriteID) { this->spriteID = spriteID; }
	FRECT GetCollisionBox() { return FRECT(); }
	void Update(float dt);
	void Render();
};
