#pragma once
#include "DynamicObject.h"

class MineBullet : public DynamicObject
{
	float baseVelocity = 50;

public:
	MineBullet();
	void Render();
	void Update(float dt);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	float getBaseVelocity() { return baseVelocity; }
};