#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Animator.h"

class Insect : public DynamicObject
{
private:
	//FlipX ( true = right, false = left )
	bool flipX = false;

	int speed = 20;
	
	//behaviour
	enum InsectState
	{
		Forceburst,
		GoUp,
		HoldHeight,
		FallDown,
	};
	InsectState state = GoUp;

	DWORD lastHoldHeightTime = -9999;

	int ColX = 0;
	int ColY = 0;

	void SetNextState();

public:
	Insect();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};

