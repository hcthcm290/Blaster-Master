#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"

class Bullet_Jason : public DynamicObject
{
private:
	int speed = 200;
	float livingTime = 5;

public:
	Bullet_Jason(int horizontalDirection, float xs, float ys);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};
