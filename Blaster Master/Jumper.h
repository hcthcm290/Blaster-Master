#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "Enemy.h"
#define jumperSpeed	30;
#define jumperWalk State::_JUMPER_WALK_
#define	jumperIdle State::_JUMPER_IDLE_

class Jumper : public DynamicObject, public Enemy
{
private:
	bool onTheGround = false;
	bool canJump = false;
	int state;
	bool flip = false;
	bool trigger = false;
	float waitForJump = 2;
	int direction = 1;
	int jumpCount=0;
	float mini_waitForJump = 0;
	float accelerateY = 700;
	float jumpTime;
public:
	Jumper();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};