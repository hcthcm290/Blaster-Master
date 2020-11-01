#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"

#define jumperSpeed	10;
#define jumperWalk Constant::_JUMPER_WALK_
#define	jumperIdle Constant::_JUMPER_IDLE_
#define accelerate 50;

class Jumper : public DynamicObject, public RigidBody
{
private:
	float currentY, Vy = 0, Vx = jumperSpeed;
	bool isJumpUp = false, isJumpDown = false;
	int state;				//Jump or walk
	float maxJumpTop = 20;		//Accelerate
	bool keyRelease = true;	//Stop click on button
	bool flip = false;		//flip sprite
	int direction = -1;		//Moving Vector

public:
	Jumper();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
};