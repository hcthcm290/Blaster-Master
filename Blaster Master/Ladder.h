#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Animator.h"
#include "Jason.h"

class Ladder : public DynamicObject
{
private:
	int height;
	Jason* jason;
	float top, bottom;
public:
	Ladder(int h);
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};

