#pragma once
#include "DynamicObject.h"
#include "Enemy.h"
#include "ColliableBrick.h"
class BreakableWall :
    public ColliableBrick
{
	int HP = 40;
public:
	BreakableWall();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void TakeDamage(int dmg);
	void Render();
};

