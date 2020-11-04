#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"

using namespace std;

class Jason : public DynamicObject, public RigidBody
{
private:
	int state;
	int speed = 200;
	int flipX = false;

	void MakeCrouch();
	void MakeMove();
	void MakeJump();

	wstring StateToString();
	//int initialY = 500; //instead of collision boxes
public:
	Jason();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};