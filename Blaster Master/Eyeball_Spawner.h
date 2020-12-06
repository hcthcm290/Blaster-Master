#pragma once

#include "DynamicObject.h"
#include "CameraBoundaryLib.h"

class EyeballSpawner : public DynamicObject
{
private:
	const float COOLDOWN = 3;
	float coolDown = 0;

	FRECT roomColBox;

	enum DIRECTION { _LEFT, _TOP, _RIGHT, _BOTTOM };
					//0		1		2		3
	const int arrDx[4] = { -1,0,1,0 };
	const int arrDy[4] = { 0,-1,0,1 };

	float left, top, right, bottom;
	float RandRange(float l, float r);
	void SetBoundary(FRECT colBox);

public:
	EyeballSpawner(float left, float top, float right, float bottom);
	void Update(float dt);
	void Render();
	//no collision stuffs
};