#pragma once

#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "StateConstant.h"
#include "PlayerBullet.h"
#include "Intangibility.h"

#pragma region Fly Pattern
//== ABOUT FLY PATTERN ==
//	Fly Patterns only support bullets make movement due to its current position:
//  Update Function in FlyPatterns are main controller of the bullet

#pragma region DEFAULT
//===	DEFAULT	===
class BulletJasonInteriorFlyPattern {
protected:
	const float BASE_SPEED = 400;
	const float BASE_LIVINGTIME = 0.3; //300m
	const float PI = 3.14;

	float vx = 1;
	float vy = 1;
	float Deg2Rad(float deg) { return deg * PI / 180; }
public:
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	virtual void Update(float dt);
};
#pragma endregion

#pragma region STRAIGHT
//===	STARIGHT 1->4 ===
class StraightPattern : public BulletJasonInteriorFlyPattern {
private:

public:
	StraightPattern(float& livingTime, int& speed, int level, int dx, int dy);
};
#pragma endregion

#pragma region CIRCLE
//===	CIRCLE	5->6 ===
class CirclePattern : public BulletJasonInteriorFlyPattern {
private:
	static int iDirection;
	static int arrDirection[4];

	int direction;
	float BASE_ANGLEOFFSET = 360 * 0.5; //180 deg per second
	float angleOffset;

	float currentAngle;

public:
	CirclePattern(float& livingTime, int& speed, int level, int dx, int dy);
	void Update(float dt);
	float GetAngle();
};
#pragma endregion

#pragma region WAVE
//===	WAVE 7->8	===
class WavePattern : public BulletJasonInteriorFlyPattern {
private:
	//adjustment between bullets
	const float DELAY_WAVE_OFFSET = 0.025; 
	const float DELAY_WAVE_MAX = 0.1;
	static float nextDelay;	

	static float nextDirection;
	float direction;

	float delay; //if the bullet is "delaying", it flies straight

	const float BASE_FI_OFFSET = 3.14 * 2; //approximately 4*PI per second
	float fiOffset; //RAD
	float fi = 0;	//RAD
	
	//at lv7, lv8 is adjusted in code
	float spreadDiv = 6;
	float spread = 1.65; 

	bool isHorizontal;
public:
	WavePattern(float& livingTime, int& speed, int level, int dx, int dy);
	void Update(float dt);
};
#pragma endregion
#pragma endregion

class BulletJasonInterior : public DynamicObject, public PlayerBullet, public Intangibility
{
private:
	int speed;

	//DAMAGE
	const int BASE_DAMAGE = 20;
	const int UPGRADE_DAMAGE = 5;
	int damage;

	int bulletLevel = 9999;
	int dx = 0, dy = 0;

	float livingTime = 0;
	BulletJasonInteriorFlyPattern* flyPattern;

	bool exploded = false;
	void Explode();

	//for Render
	float GetRotation();
	D3DCOLOR GetColor();

	//color changing (LVL8)
	D3DCOLOR arrColor[8] = {
		D3DCOLOR_ARGB(255, 255, 0, 0), //red
		D3DCOLOR_ARGB(255, 255, 165, 0), //orange
		D3DCOLOR_ARGB(255, 255, 255, 0), //yellow
		D3DCOLOR_ARGB(255, 0, 255, 0), //green
		D3DCOLOR_ARGB(255, 0, 0, 255), //blue
		D3DCOLOR_ARGB(255, 75, 0, 130), //indigo
		D3DCOLOR_ARGB(255, 128, 0, 128), //violet
		D3DCOLOR_ARGB(255, 255, 255, 255), //white
	};
	int color = 7; //default: WHITE
	const float COLOR_DELAY = 0.05;
	float colorDelay = 0;

public:
	BulletJasonInterior(int horizontal, int vertical, int level);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	//avoidind dysfunction bullets
	bool Invalid() { return bulletLevel <= 8 && bulletLevel > 0; }
	bool IsExploded() { return exploded; }
};