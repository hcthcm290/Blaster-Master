#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "PlayerBullet.h"

class Grenade : public DynamicObject, public PlayerBullet
{
private:
	float livingTime = 0.25;
	float PlayerY;
	bool Horizontal = false;
	bool boom = false;
public:
	Grenade(bool);
	void Remove();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void DealDamage();
};
#pragma once
