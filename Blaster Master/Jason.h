#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Ladder.h"
#include <d3dx9.h>
#include "Playable.h"

using namespace std;

class Jason : public DynamicObject, public Playable
{
private:
	//const
	const int MAXHP = 40;
	const int GRAVITY = 300;

	//bacic properties
	const int speed = 5000;
	const int jumpForce = 150;
	bool flipX = false;

	DWORD lastFire = 0;
	float recoilTime = 500;

	DynamicObject* sophia;

	//ACTION RECORD vairable to set new state
	int horizontalMove;
	int verticalMove;
	bool attemptJump;
	float enviColX;
	float enviColY;
	float enemyColX;
	float enemyColY;

	bool allowShoot = true;
	bool allowJump = false;

	void UpdateActionRecord();
	void SetNewState();

	void Fire();

	//JUMPTIMER - inflict damage if overtime
	const float MAX_JUMPTIME = 1.0f;
	float jumpTime = 0;

	DWORD switchDelay; //this avoid switching back and forth too fast 

	//on damage
	int invulnerable = -1; //trigger color-changing effect (-1: off, 0:green, 1: magneta)
	DWORD lastTakeDamage = 0;
	float invulnerableTime = 500;
	static constexpr D3DCOLOR damageColor[] = {
		D3DCOLOR_ARGB(255,148,247,207),
		D3DCOLOR_ARGB(128,124,124,124),
		D3DCOLOR_ARGB(255,247,164,143),
		D3DCOLOR_ARGB(128,124,124,124),
	};
	int damageEffectTimer = -999;

	float deathAnimationCountdown;


	#pragma region LADDER VARS
	Ladder* ladder;
	LadderPos ladderPos;

	float switchEffectDuration = -1;
	float flipCountDown = 0.25f;
	#pragma endregion

private:
	int bulletPower;

public:
	Jason();
	Jason(int currentHealth, int x, int y, DynamicObject* sophia);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	virtual bool IsInvulnerable() { return this->invulnerable != -1; }
	float GetEnterGateSpeed();
	void PushBack(int _vx, int _vy);

	//void PushBack(int _vx, int _vy);
	void TakeDamage(int dmg);
	int GetState() { return currentState; }
	void SetState(int state) { this->currentState = state; }
	void SetBulletPower(int value) { this->bulletPower = value; }
	int GetBulletPower() { return this->bulletPower; }
	bool IsDead();
};
