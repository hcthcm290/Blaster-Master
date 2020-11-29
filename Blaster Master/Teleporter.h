#pragma once
#include "DynamicObject.h"
#include "Enemy.h"
class Teleporter :
    public DynamicObject, public Enemy
{
public:
	Teleporter();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};

