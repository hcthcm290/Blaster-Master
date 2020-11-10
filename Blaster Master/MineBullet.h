#pragma once
#include "DynamicObject.h"
#include "Intangibility.h"

class MineBullet : public DynamicObject, public Intangibility
{
	float baseVelocity = 30;

public:
	MineBullet();
	void Render();
	void Update(float dt);
	FRECT GetCollisionBox();
	float getBaseVelocity() { return baseVelocity; }
};