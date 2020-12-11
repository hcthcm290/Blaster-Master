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
	//bacic properties

	int speed = 100;
	int jumpSpeed = 100;
	int flipX = false;

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
	void TakeDamage(int dmg);
	int GetState() { return currentState; }
	void SetBulletPower(int value) { this->bulletPower = value; }
	int GetBulletPower() { return this->bulletPower; }
};
