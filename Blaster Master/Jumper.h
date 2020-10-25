#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"

class Jumper : public DynamicObject,public RigidBody
{
public:
	bool controlable = false;
public:
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
};