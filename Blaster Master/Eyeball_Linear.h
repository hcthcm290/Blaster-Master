#pragma once
#include "DynamicObject.h"
#include "ColliableBrick.h"

class EyeballMovePath;

class EyeballLinear : public DynamicObject
{
private:
	const float PI = 3.14;
	float Deg2Rad(int deg) { return deg * PI / 180; }

	const int SPEED = 300;

	const float LIVING_TIME = 5; //5 seconds
	float livingTime;

	float speed;
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