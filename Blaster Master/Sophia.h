#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "Jason.h"
#include "CollisionSystem.h"
#include "Playable.h"

class Sophia : public DynamicObject, public Playable
{
	//
	int invincible = 0;
	//
	bool onTheGround = true;
	bool canJump = false;
	int state;
	DWORD last;
	bool flipX;
	bool moving;
	int gun_up;
	DWORD last_gun;
	int sx;
	int sy;
	DWORD start_gunturn;
	bool last_flipX;
	bool gun_turn;
	DWORD start_shift;
	DWORD last_bullet;
	static constexpr D3DCOLOR damageColor[] = {
	D3DCOLOR_ARGB(255,255,255,255),
	D3DCOLOR_ARGB(255,148,247,207),
	D3DCOLOR_ARGB(128,124,124,124),
	D3DCOLOR_ARGB(255,247,164,143),
	D3DCOLOR_ARGB(128,124,124,124),
	};
	int currentColor;
	DWORD lastDamageTime;
	
	DWORD die;

	//JASON
	Jason* jason;
	int JasonCurrentHealth = 40;

	DWORD switchDelay; //avoid switching back and forth so fast

	bool switching = true;

	// Weapon type
	// 1: Homing
	// 2: Thunder
	// 3: Rocket
	int currentWeapon = 1; 
public:
	Sophia();

	int Homing = 20;
	int Thunder = 20;
	int Rocket = 5;

	bool isInvincible();
	virtual bool IsInvulnerable() { return isInvincible(); }
	float GetEnterGateSpeed();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
	void StateChange();
	void ShootHoming();
	void ShootThunder();
	void ShootRocket();
	~Sophia();// Trung Nguyễn
	void TakeDamage(int dmg);
	bool IsDead();

	void Awake(int JasonHealth);
	virtual int GetState() { return this->state; }
	virtual void SetState(int state) { this->state = state; }

	int GetCurrentWeapon() { return this->currentWeapon; }
	void SetCurrentWeapon(int value) { this->currentWeapon = value; }
	int GetNumberOfHoming() { return this->Homing; }
	int GetNumberOfThunder() { return this->Thunder; }
	int GetNumberOfRocket() { return this->Rocket; }
	void SetNumberOfHoming(int value) { this->Homing = value; }
	void SetNumberOfThunder(int value) { this->Thunder = value; }
	void SetNumberOfRocker(int value) { this->Rocket = value; }
};

