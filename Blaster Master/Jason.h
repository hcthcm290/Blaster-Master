#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Ladder.h"
#include <d3dx9.h>

using namespace std;

class Jason : public DynamicObject
{
private:
	//bacic properties

	static const int maxHealth = 6;
	int health = 6;

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
		D3DCOLOR_ARGB(255,0,255,0),
		D3DCOLOR_ARGB(255,255,0,255)
	};
	int damageEffectTimer = -999;


	#pragma region LADDER VARS
	Ladder* ladder;
	LadderPos ladderPos;

	float switchEffectDuration = -1;
	float flipCountDown = 0.25f;
	#pragma endregion

public:
	Jason();
	Jason(int currentHealth, int x, int y, DynamicObject* sophia);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	void PushBack(int _vx, int _vy);
	void getDamage(int damage);

	int GetState() { return currentState; }
};
