#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
#include "Intangibility.h"
class Sophia_Bullet_Rocket :
	public DynamicObject, public Bullet, public Intangibility
{
protected:
	DWORD last;
	int state;
	bool ex;
	bool flipX;
	bool up;
	bool down;
	DWORD last_anim;
	int temp;
	int num;
	float time = 0;
public:
	static int count;
	Sophia_Bullet_Rocket();
	Sophia_Bullet_Rocket(bool flipX, int num);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Explode();
	void Update(float dt);
	void Render();
	void RemoveBullet();
};

