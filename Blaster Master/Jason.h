#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"

using namespace std;

class PlayerInput {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool shoot = false;
	void Update() {
		up = DInput::GetInstance()->KeyDown(DIK_UP);
		down = DInput::GetInstance()->KeyDown(DIK_DOWN);
		left = DInput::GetInstance()->KeyPress(DIK_LEFT);
		right = DInput::GetInstance()->KeyPress(DIK_RIGHT);
		jump = DInput::GetInstance()->KeyDown(DIK_X);
		shoot = DInput::GetInstance()->KeyDown(DIK_Z);
	}
};

class Jason : public DynamicObject, public RigidBody
{
private:
	//bacic properties
	int state;
	int health = 6;
	int speed = 120;
	int flipX = false;

	PlayerInput* input;

	//ACTION RECORD vairable to set new state
	int horizontalMove;
	int verticalMove;
	bool attemptJump;
	float enviColX;
	float enviColY;
	bool outOfHealth;
	float enemyColX;
	float enemyColY;

	void UpdateActionRecord();
	int SetNewState();

	wstring StateToString();
	//int initialY = 500; //instead of collision boxes
public:
	Jason();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};

#define OFFSET_STAND_CRAWL 10;