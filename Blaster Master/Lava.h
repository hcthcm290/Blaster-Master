#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Animator.h"

class Lava : public DynamicObject
{
private:
	bool flipX = true;
	int flipCountDown;
	int length;
public:
	Lava(int length);
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};

