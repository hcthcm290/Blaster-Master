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

	static const int maxHealth = 6;
	int health = 6;

	int speed = 100;
	int flipX = false;

	DWORD lastTakeDamage = 0;
	float invulnerableTime = 500;

	DWORD lastFire = 0;
	float recoilTime = 500;

	PlayerInput* input = new PlayerInput();

	//ACTION RECORD vairable to set new state
	int horizontalMove;
	int verticalMove;
	bool attemptJump;
	float enviColX;
	float enviColY;
	float enemyColX;
	float enemyColY;
	bool allowJump = false;

	void UpdateActionRecord();
	void SetNewState();

	void Fire();

public:
	Jason();
	Jason(int currentHealth, int x, int y);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	void PushBack(int _vx, int _vy);
	void getDamage(int damage);
};
