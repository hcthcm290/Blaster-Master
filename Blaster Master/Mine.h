#pragma once
#include "DynamicObject.h"
#include "Enemy.h"

class Mine : public DynamicObject, public Enemy
{
private:
	bool active = false;
	int damage = 10;
	float waiting = 0;
public:
	Mine();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();

	void OnCollisionEnter(CollisionEvent e);

	void TakeDamage(int dmg);
};

