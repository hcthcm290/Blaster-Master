#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
class Skull_Bullet :
    public DynamicObject, public Bullet
{
protected:
	DWORD last;
	int state;
	bool ex;
public:
	Skull_Bullet();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Explode();
	void Update(float dt);
	void Render();
};

