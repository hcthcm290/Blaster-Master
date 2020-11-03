#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"

#define ORB_COLLISION_BOX_WIDTH 18
#define ORB_COLLISION_BOX_HEIGHT 18

class Orb : public DynamicObject, public RigidBody
{
	bool onTheGround = true;
	bool canJump = false;

public:
	Orb();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};