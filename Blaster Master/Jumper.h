#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#define jumperSpeed	10;
#define jumperWalk State::_JUMPER_WALK_
#define	jumperIdle State::_JUMPER_IDLE_
#define accelerate 50;

class Jumper : public DynamicObject, public RigidBody
{
private:
	bool onTheGround = true;
	bool canJump = true;
	int state;
	bool flip = false;
	bool trigger = false;
public:
	Jumper();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};