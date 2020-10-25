#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"

#define ORB_COLLISION_BOX_WIDTH 18
#define ORB_COLLISION_BOX_HEIGHT 18

class Orb : public DynamicObject,public RigidBody
{
public:
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};