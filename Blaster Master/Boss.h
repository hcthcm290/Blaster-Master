#pragma once

#include "DynamicObject.h"
#include "Enemy.h"
#include "Intangibility.h"
#include "Bullet_Eyeball.h"

// PRE DECLARATIONS
class BossArm;
class BossHand;

#pragma region BOSS
/** ===== BOSS ===== **/
const float BOSS_MAX_HEALTH = 100;//2000;
const float BOSS_SPEED = 1000;
const float BOSS_FIRE_COOLDOWN = 2;
const float BOSS_FIRE_SPEED = 0.25;

class Boss : public DynamicObject, public Enemy 
{
private:
	bool dead = false;
	DWORD deadTime;
	//velocity
	float vx0, vy0;

	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(128,255,255,255),
	D3DCOLOR_ARGB(128,0,0,0)
	};

	static constexpr D3DCOLOR deadColor[] = {
	D3DCOLOR_ARGB(255,255,0,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,255,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,0,255),
	D3DCOLOR_ARGB(128,0,0,0),
	};

	DWORD lastex;

	//sub-gameObjects
	//<[0] means RIGHT> - and - <[1] means LEFT>
	BossHand* arrBossHand[2];
	
	//Fire
	int bulletRemaining;
	float fireTimer;
	void CheckFire(float dt);
	void Fire();

	//BossHand Controller
	float lShoulderX() { return x - 20; }
	float rShoulderX() { return x + 20; }

public: 
	Boss(float x, float y);

	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	void TakeDamage(int dmg);
};
#pragma endregion

const float ARM_SPEED = 10000;
const float MIN_MOVEMENT_VELOCITY = 5;
#pragma region BOSS_ARM
/** ===== BOSS ARM ===== **/
class BossArm : public DynamicObject, public Intangibility
{
private:

	float vx0, vy0;
	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(128,255,255,255),
	D3DCOLOR_ARGB(128,0,0,0)
	};
	static constexpr D3DCOLOR deadColor[] = {
	D3DCOLOR_ARGB(255,255,0,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,255,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,0,255),
	D3DCOLOR_ARGB(128,0,0,0),
	};
public:
	#pragma region (Deprecated)
	void Render();
	#pragma endregion

	bool dead = false;
	BossArm();
	void Update(float dt);
	void Render(bool flipX);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	float GetX() { return this->x; };
	float GetY() { return this->y; };

	void SetDestination(float newX, float newY);
};
#pragma endregion

#pragma region BOSS_HAND
/** ===== BOSS HAND ===== **/
class BossHand : public DynamicObject, public Intangibility
{
private:
	//arm control
	BossArm* arrBossArm[4];

	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(128,255,255,255),
	D3DCOLOR_ARGB(128,0,0,0)
	};
	static constexpr D3DCOLOR deadColor[] = {
	D3DCOLOR_ARGB(255,255,0,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,255,0),
	D3DCOLOR_ARGB(128,0,0,0),
	D3DCOLOR_ARGB(255,0,0,255),
	D3DCOLOR_ARGB(128,0,0,0),
	};

	//offset due to shoulder
	const float DISTANCE = 60;
	const float EPSILON = MIN_MOVEMENT_VELOCITY + 5;
	float offsetX = 0, offsetY = 0;

	//velocity
	float vx0, vy0;

	//geometry functions
	#pragma region RandRange(l,r)
	float RandRange(float l, float r) {
		int d = (int)(r - l + 2); //l -> r
		return (float)(l + (rand() % d));
	}
	#pragma endregion
	#pragma region GetDistance(x1,y1,x2,y2)
	float GetDistance(float x1, float y1, float x2, float y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}
	#pragma endregion

	void UpdateArms(float sx, float sy);

	int directionX, directionY;
	DWORD freezeX, freezeY;
	bool isFreezeX = false, isFreezeY = false;

public:
	#pragma region (Deprecated)
	void Render();
	#pragma endregion

	bool dead = false;
	BossHand(float x, float y);
	void Update(float dt);
	void Render(bool flipX);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	float GetX() { return this->x; };
	float GetY() { return this->y; };

	void SetNextPosition(float shoulderX, float shoulderY, bool isLeft);

	//
	void ChainArm(BossArm* arm, BossHand* hand);
	void ChainArm(BossArm* arm, float sx, float sy);
	void ChainArm(BossArm* arm, BossArm* armcheck);
	float CalculateDistance(float a, float b, float d);
};
#pragma endregion

class ExplosiveBoss : public DynamicObject
{
private:
	DWORD creationTime;
public:
	ExplosiveBoss(int x, int y);
	void Render();
	void Update(float dt);
};
