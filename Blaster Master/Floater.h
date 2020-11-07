#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"

#define floaterFly State::_FLOATER_FLY_
#define	floaterIdle State::_FLOATER_IDLE_
#define	floaterSpeed State::_FLOATER_SPEED_

class Floater : public DynamicObject, public RigidBody
{
private:
	int state;
	bool flip = true;
	bool trigger = true;
public:
	Floater();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};