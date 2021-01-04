#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"

class Bullet_Eyeball : public DynamicObject {
private:
	const float SPEED = 3500;
	float livingTime = 3000;

	float vx0;
	float vy0;

	void Explode();

public:
	Bullet_Eyeball(float x, float y);
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};
