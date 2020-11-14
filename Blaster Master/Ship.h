#pragma once
#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Enemy.h"

#define shipFly State::_FLOATER_FLY_
#define	shipIdle State::_FLOATER_IDLE_
#define	shipSpeed State::_FLOATER_SPEED_

class Ship : public DynamicObject, public Enemy
{
private:
	int state;
	bool flip = true;
	bool trigger, fly = true;
	int shotCount = 0;
	float waitForShot = 5;
	float mini_waitForShot = 0.5;
public:
	Ship();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};