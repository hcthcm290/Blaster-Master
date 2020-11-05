#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#define jumperSpeed	10;
#define jumperWalk Constant::_JUMPER_WALK_
#define	jumperIdle Constant::_JUMPER_IDLE_
#define accelerate 50;

class Jumper : public DynamicObject, public RigidBody
{
private:
	bool onTheGround = true;
	bool canJump = false;
	int state;
	bool flip = false;
	bool trigger = false;
	float waitForJump = 2;
	int direction = 0;
public:
	Jumper();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};