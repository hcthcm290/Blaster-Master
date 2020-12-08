#pragma once
#include "DynamicObject.h"
#include "Camera.h"

class EyeballCurved : public DynamicObject
{
private:
	const float PI = 3.14;
	float Deg2Rad(float deg) { return deg * PI / 180; }

	const float MAX_SPEED = 4000;
	const float MIN_SPEED =	1000;
	float speed;

	DynamicObject* player;

	bool isResting = true; //standstill mode
	const float MAX_SWITCH_TIME = 2; //2 seconds
	float switchTime;

	void Die();

	//FIRE
	void Fire();

	//DIRECTION
	enum DIRECTION { _UP, _LEFT, _DOWN, _RIGHT, _NULL };
	pair<DIRECTION, DIRECTION> direction = { _NULL, _NULL };
	pair<DIRECTION, DIRECTION> GetDirection();

	//UPDATE
	void CheckDirection();
	void UpdateMovement();
	void SetStartVelocity();

	//ANGLE
	const float MIN_ANGLE_OFFSET = 45;
	const float MAX_ANGLE_OFFSET = 180;
	float angleOffset;

	const float ADJUSTMENT_VARIABLE = 0.8;

	//calculate distance from eyeball to player
	float dx, dy;

	//timing
	float startTurning;
	float endTurning;

	//support
	const float MAX_L = 96;
	float vx0, vy0; //default
public:
	EyeballCurved();

	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};