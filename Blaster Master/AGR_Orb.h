#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "Enemy.h"

#define ORB_COLLISION_BOX_WIDTH 18
#define ORB_COLLISION_BOX_HEIGHT 18
#define orbSpeed	100
#define orbIdle State::_AGR_ORB_IDLE_
#define orbFly State::_AGR_ORB_FLY_

class AGR_Orb : public DynamicObject, public Enemy
{
	int state;
	bool trigger = false;
public:
	AGR_Orb();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};	