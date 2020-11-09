#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Animator.h"

class Insect : public DynamicObject, public RigidBody
{
private:
	bool flipX = false;
	
	//behaviour
	enum InsectState
	{
		GoUp,
		HoldHeight,
		FallDown,
	};
	InsectState state = GoUp;

	DWORD lastTime = 0;

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

