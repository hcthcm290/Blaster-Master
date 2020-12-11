#pragma once
#include "GameObject.h"

class StaticObject : public CGameObject
{
protected:
	int spriteID;

public:
	void SetSpriteID(int spriteID) { this->spriteID = spriteID; }
	virtual FRECT GetCollisionBox() { return FRECT(); }
	virtual void Update(float dt);
	virtual void Render();
};
