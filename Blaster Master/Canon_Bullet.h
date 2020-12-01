#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "InteriorScene.h"
#include "GameObject.h"

#define canonBullet State::_CANON_BULLET_

class Canon_Bullet : public DynamicObject
{
private:
	bool flip = true;
	int state;
	bool boom = false;
public:
	Canon_Bullet();
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};