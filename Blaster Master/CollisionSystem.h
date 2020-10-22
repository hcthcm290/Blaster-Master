#pragma once
#include "DynamicObject.h"

struct Collision;
typedef Collision* LPCOLLISION;
struct Collision
{
	LPGAMEOBJECT obj;
	float dt_Percent, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	Collision(float dt_Percent, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
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
	LPGAMEOBJECT pGameObject;

	CollisionEvent(LPGAMEOBJECT pGameObject)
	{
		this->pGameObject = pGameObject;
	}
};


class CollisionSystem
{
public:
	/// <summary>
	/// Move object based on their velocity then apply collision to them
	/// </summary>
	/// <param name="dynamicObj">The object that move</param>
	/// <param name="anotherObjs">List of all the object that moving object may interact with</param>
	/// <param name="dt"></param>
	static void DoCollision(DynamicObject* dynamicObj, std::vector<CGameObject*>* anotherObjs, float dt);

private:
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);
	static LPCOLLISION SweptAABBEx(DynamicObject* movingObj, CGameObject* staticObj, float dt);
	static std::pair< LPCOLLISION, LPCOLLISION> FilterCollisions(std::vector<LPCOLLISION> eventList);
};