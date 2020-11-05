#pragma once

#include "DynamicObject.h"









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

	static bool CheckOverlap(CGameObject* obj1, CGameObject* obj2);



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

	static std::pair<std::vector<LPCOLLISION>, std::vector<LPCOLLISION>> FilterCollisions(DynamicObject* movingObj, std::vector<LPCOLLISION> eventList, float dt);

};