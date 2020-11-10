#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Jason.h"
#include "CollisionSystem.h"

class Sophia : public DynamicObject, public RigidBody
{
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

	//JASON
	Jason* jason;
	int JasonCurrentHealth = 6;
public:
	Sophia();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
	void StateChange();

	void Awake(int JasonHealth);
};

