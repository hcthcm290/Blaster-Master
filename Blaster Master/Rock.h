#pragma once
#include "DynamicObject.h"
#include "Enemy.h"
#include "ColliableBrick.h"
class Rock :
    public ColliableBrick
{
	int HP = 40;
public:
	Rock();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void TakeDamage(int dmg);
};

