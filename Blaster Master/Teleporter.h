#pragma once
#include "DynamicObject.h"
#include "Enemy.h"
class Teleporter :
    public DynamicObject, public Enemy
{
protected:
	int state;
	bool invincible = true;
	int cooldown = 500;
	int step = 5;
	int sx, sy;
	int ox, oy;
	FRECT zone;
	DWORD start_tele;
public:
	Teleporter(FRECT zone);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
	void TakeDamage(int dmg);
};

