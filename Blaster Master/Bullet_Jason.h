#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayerBullet.h"

class Bullet_Jason : public DynamicObject, public PlayerBullet
{
private:
	int speed = 150;
	float livingTime = 0.5;

public:
	Bullet_Jason(int horizontalDirection, float xs, float ys);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void OnOverlap(CGameObject* obj);
};
