#pragma once
#include "DynamicObject.h"

class Jason : public DynamicObject
{
private:
	int state;
	int speed = 2;
	int flipX = false;

	void MakeCrouch();
	void MakeMove();
	void MakeJump(float dt);
	void GravityEffect();

	int initialY = 500; //instead of collision boxes
public:
	Jason();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
};