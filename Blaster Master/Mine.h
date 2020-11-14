#pragma once
#include "DynamicObject.h"
#include "Enemy.h"

class Mine : public DynamicObject, public Enemy
{
private:
	bool active = false;

public:
	Mine();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();

	void OnCollisionEnter(CollisionEvent e);
};

