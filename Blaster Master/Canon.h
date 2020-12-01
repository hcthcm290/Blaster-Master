#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Enemy.h"

#define canon State::_CANON_
#define canonHor State::_CANON_HOR_
#define canonVer State::_CANON_VER_


class Canon : public DynamicObject, public Enemy
{
	private:
		int state;
		float waitForShot = 1;
		bool verFirst = true;
	public:
		Canon();
		void Update(float dt);
		void Render();
		FRECT GetCollisionBox();
};

