#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
class Explosive_Cannon_Bullet :
	public DynamicObject, public Bullet
{
	DWORD last;
public:
	Explosive_Cannon_Bullet();
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};

#pragma once
