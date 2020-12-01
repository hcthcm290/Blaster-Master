#pragma once
#include "DynamicObject.h"
#include "Playable.h"
#include "Jason_BulletManager_Interior.h"

class BigJason : public DynamicObject, public Playable
{
private:
	Jason_BulletManager* bulletManager = new Jason_BulletManager();
public:
	BigJason();
	void Update(float dt);
	void Render();
	virtual FRECT GetCollisionBox();
	bool IsInvulnerable();
};
