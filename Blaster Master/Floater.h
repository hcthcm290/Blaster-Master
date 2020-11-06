#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"

#define floaterFly State::_FLOATER_FLY_
#define	floaterIdle State::_FLOATER_IDLE_

class Floater : public DynamicObject, public RigidBody
{
private:

	int state;
	bool flip = false;
	int direction = 0;
public:
	Floater();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};