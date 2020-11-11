#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
class Sophia_Bullet_1 :
    public DynamicObject, public Bullet
{
protected:
	DWORD last;
	int state;
	bool ex;
	bool flipX;
	bool up;
public:
	static int count;
	Sophia_Bullet_1();
	Sophia_Bullet_1(bool up, bool flipX);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Explode();
	void Update(float dt);
	void Render();
	void RemoveBullet();

};
