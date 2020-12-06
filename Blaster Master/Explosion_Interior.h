#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"

class ExplosionInterior : public DynamicObject
{
private:
	const float MAX_SHAKE_WAIT_TIME = 0.075f;
	float shakingWaitTime = MAX_SHAKE_WAIT_TIME; //make shaking effect

	const int SHAKE_OFFSET = 1;

	float livingTime = MAX_SHAKE_WAIT_TIME * 3;
public:
	ExplosionInterior(int x, int y);
	void Update(float dt);
	void Render();
};
