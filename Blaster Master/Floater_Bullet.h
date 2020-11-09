#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"

#define floaterBullet State::_FLOATER_BULLET_

class Floater_Bullet : public DynamicObject
{
private:
	bool flip = true;
	int state;
	bool boom = false;
public:
	Floater_Bullet();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};