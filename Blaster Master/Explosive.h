#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
class Explosive :
    public DynamicObject
{
	DWORD last;
public:
	Explosive();
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};

