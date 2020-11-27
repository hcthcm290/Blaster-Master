#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayerBullet.h"
#include "Intangibility.h"
class Sophia_Bullet_Homing :
    public DynamicObject, public PlayerBullet, public Intangibility
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
	DynamicObject* target;
public:
	static int count;
	Sophia_Bullet_Homing();
	Sophia_Bullet_Homing(bool flipX, DynamicObject* target);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Explode();
	void Update(float dt);
	void Render();
	void RemoveBullet();
};

