#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"

#define floaterBullet State::_FLOATER_BULLET_

class Floater_Bullet : public DynamicObject, public RigidBody
{
private:
	bool flip = true;
	int state;
public:
	Floater_Bullet();
	Floater_Bullet(float, float );
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};