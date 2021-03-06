#pragma once
#include "DynamicObject.h"
#include "ColliableBrick.h"
#include "Enemy.h"

class Dome : public DynamicObject, public Enemy
{
private:
	D3DXVECTOR2 gravity;
	D3DXVECTOR2 direction;
	float soundCount = 1;
	float movingSpeed = 50;
	float flyingSpeed = 200;

	float waitToFly = 1;
	float flyReloadTime = 1;

	std::vector<ColliableBrick*> ground;
public:
	Dome() {};
	Dome(int id_gravity, int id_direction);
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};