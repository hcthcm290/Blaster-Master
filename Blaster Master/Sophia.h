#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Jason.h"
#include "CollisionSystem.h"

class Sophia : public DynamicObject
{
	//
	int invincible = 0;
	//
	bool onTheGround = true;
	bool canJump = false;
	int state;
	DWORD last;
	bool flipX;
	bool moving;
	int gun_up;
	DWORD last_gun;
	int sx;
	int sy;
	DWORD start_gunturn;
	bool last_flipX;
	bool gun_turn;
	DWORD start_shift;
	DWORD last_bullet;

	//JASON
	Jason* jason;
	int JasonCurrentHealth = 100;

	DWORD switchDelay; //avoid switching back and forth so fast
public:
	Sophia();

	bool isInvincible();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
	void StateChange();
	void ShootHoming();

	void TakeDamage(int dmg);

	void Awake(int JasonHealth);
};

