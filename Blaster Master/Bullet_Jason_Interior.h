#pragma once

#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "StateConstant.h"
#include "PlayerBullet.h"

#pragma region Fly Pattern
//== ABOUT FLY PATTERN ==
//	Fly Patterns only support bullets make movement due to its current position:
//	vx, vy of BulletJasonInterior make bullets go ahead
//	vx, vy of FlyPatterns add additional movement to the bullet

//===	DEFAULT	===
class BulletJasonInteriorFlyPattern {
protected:
	const float BASE_SPEED = 300;
	const float BASE_LIVINGTIME = 15; //150ms, idk why

	float vx = 0;
	float vy = 0;
public:
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	virtual void Update(float dt);
};

//===	STARIGHT 1->4 ===
class StraightPattern : public BulletJasonInteriorFlyPattern {
private:

public:
	StraightPattern(int& livingTime, int& speed, int level, int dx, int dy);
};

//===	CIRCLE	5->6 ===
class CirclePattern : public BulletJasonInteriorFlyPattern {
private:
	const float PI = 3.14;
	float BASE_ANGLEOFFSET = 360; //2 round per second
	float angleOffset;

	float currentAngle;
	float Deg2Rad(float deg) { return deg * PI / 360; }
public:
	CirclePattern(int& livingTime, int& speed, int level, int dx, int dy);
	void Update(float dt);
	float GetAngle() { return currentAngle; }
};

//===	WAVE 7->8	===
class WavePattern : public BulletJasonInteriorFlyPattern {
private:
	const float BASE_FI_OFFSET = 4.71; //approximately 1.5*PI per second
	float fiOffset;
	float fi = 0;
	
	bool isHorizontal;
public:
	WavePattern(int& livingTime, int& speed, int level, int dx, int dy);
	void Update(float dt);
};
#pragma endregion

class BulletJasonInterior : public DynamicObject, public PlayerBullet
{
private:
	int speed;

	//DAMAGE
	const int BASE_DAMAGE = 10;
	const int UPGRADE_DAMAGE = 5;
	int damage;

	int bulletLevel = 9999;
	int dx = 0, dy = 0;

	int livingTime = 0;
	BulletJasonInteriorFlyPattern* flyPattern;

	void Explode();

	//for Render
	float GetRotation();
	D3DCOLOR GetColor();

public:
	BulletJasonInterior(int horizontal, int vertical, int level);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	//avoidind dysfunction bullets
	bool Invalid() { return bulletLevel <= 8; }
};