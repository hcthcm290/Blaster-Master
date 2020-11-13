#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Enemy.h"
class Worm : public DynamicObject, public Enemy
{
	bool flipX;
	bool onTheGround = false;
public:
	Worm();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};

