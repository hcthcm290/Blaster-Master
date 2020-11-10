#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
class Explosive :
    public DynamicObject, public Bullet
{
	DWORD last;
public:
	Explosive();
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};

