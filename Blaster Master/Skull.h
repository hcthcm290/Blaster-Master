#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Enemy.h"
class Skull :
    public DynamicObject, public Enemy
{
	bool flipX;
	int state;
public:
	bool sleep;
	DWORD last;

	Skull();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
	void DropBomb();
};

