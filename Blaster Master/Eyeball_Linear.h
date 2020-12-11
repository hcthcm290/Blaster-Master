#pragma once
#include "DynamicObject.h"
#include "ColliableBrick.h"
#include "Intangibility.h"

class EyeballLinear : public DynamicObject, public Intangibility
{
private:
	const float PI = 3.14;
	float Deg2Rad(float deg) { return deg * PI / 180; }

	const float SPEED = 1e4;

	const float LIVING_TIME = 5; //5 seconds
	float livingTime;
	float vx0, vy0;

	ColliableBrick* brick;

	void Die();
public:
	EyeballLinear(float x, float y, int dx, int dy);
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};