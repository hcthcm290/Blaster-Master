#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "Enemy.h"

#define ORB_COLLISION_BOX_WIDTH 18
#define ORB_COLLISION_BOX_HEIGHT 18
#define orbSpeed	50
#define orbUOD State::_ORB_UOD_
#define orbFly State::_ORB_FLY_

class Orb : public DynamicObject, public Enemy
{
	bool flip = false;
	int state;
	int hor_direction=-1;
	int ver_direction=-1;
	float waitForChange = 0;
	float turnAround = 0;
	bool hitWall = false;
public:
	Orb();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};