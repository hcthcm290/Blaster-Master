#pragma once
#include <stdio.h>

// We need to foward declaration for CGameObject
class CGameObject;

class Collision;
typedef Collision* LPCOLLISION;
class Collision
{
public:
	CGameObject* obj;
	float dt_Percent, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	Collision(float dt_Percent, float nx, float ny, float dx = 0, float dy = 0, CGameObject* obj = NULL)
	{
		this->dt_Percent = dt_Percent;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISION& a, LPCOLLISION& b)
	{
		return a->dt_Percent < b->dt_Percent;
	}
};

class CollisionEvent
{
public:
	CGameObject* pGameObject;
	float nx, ny;

	CollisionEvent() {}

	CollisionEvent(Collision* collision)
	{
		this->pGameObject = collision->obj;
		this->nx = collision->nx;
		this->ny = collision->ny;
	}
};
