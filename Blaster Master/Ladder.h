#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Animator.h"

enum class LadderPos { Top, Body, Bottom, Null };

class Ladder : public DynamicObject
{
private:
	int height;
	float top, bottom;
public:
	Ladder(int h);
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	LadderPos CheckLadderPos(int jasonState, FRECT jasonColBox                   );
};

