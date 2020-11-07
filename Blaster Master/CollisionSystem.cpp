#include "CollisionSystem.h"
#include <algorithm>
#include "RigidBody.h"
#include "Debug.h"
#include "Orb.h"
#include "Jumper.h"
#include "ColliableBrick.h"

#include "Jason.h"
#include "Debug.h"
#include "Intangibility.h"

void CollisionSystem::DoCollision(DynamicObject* movingObj, std::vector<CGameObject*>* anotherObjs, float dt)
{
	FixPreOverlapped(movingObj, anotherObjs);

	vector<LPCOLLISION> collisions;

	for (UINT i = 0; i < anotherObjs->size(); i++)
	{
		if (movingObj == anotherObjs->at(i)) continue; // obj dont do collision with itself

		LPCOLLISION e = SweptAABBEx(movingObj, anotherObjs->at(i), dt);

		if (e->dt_Percent > 0 && e->dt_Percent <= 1.0f)
		{
			collisions.push_back(e);
		}
		else
			delete e;
	}

	auto filteredCol = FilterCollisions(movingObj, collisions, dt);

	float dtx_Percent = 1;
	float dty_Percent = 1;
	float nx_pushback = 0;
	float ny_pushback = 0;

	// deal with collision object at x axis
	if (filteredCol.first.size() != 0) 
	{
		// only push moving obj back if both of them have rigidbody
		for (auto Event : filteredCol.first)
		{
			if (dynamic_cast<ColliableBrick*>(Event->obj) != NULL && dynamic_cast<Intangibility*>(movingObj) == NULL)
			{
				dtx_Percent = Event->dt_Percent;
				nx_pushback = Event->nx;
			}
		}
	}

	// deal with collision object at y axis
	if (filteredCol.second.size() != 0)
	{
		// only push moving obj back if both of them have rigidbody
		for (auto Event : filteredCol.second)
		{
			if (dynamic_cast<ColliableBrick*>(Event->obj) != NULL && dynamic_cast<Intangibility*>(movingObj) == NULL)
			{
				dty_Percent = Event->dt_Percent;
				ny_pushback = Event->ny;
			}
		}
	}

	auto movingObjVEL = movingObj->GetVelocity();
	auto movingObjPOS = movingObj->GetPosition();

	movingObj->SetPosition(movingObjPOS.x + nx_pushback * 0.2, movingObjPOS.y + ny_pushback * 0.2);
	movingObj->SetVelocity(movingObjVEL.x * dtx_Percent, movingObjVEL.y * dty_Percent);

	if (filteredCol.first.size() != 0)
	{
		for (auto Event : filteredCol.first)
		{
			movingObj->OnCollisionEnter(CollisionEvent(Event));

			CollisionEvent e;
			e.nx = -Event->nx;
			e.ny = -Event->ny;
			e.pGameObject = movingObj;

			Event->obj->OnCollisionEnter(e);
		}
	}
	if (filteredCol.second.size() != 0)
	{
		for (auto Event : filteredCol.second)
		{
			movingObj->OnCollisionEnter(CollisionEvent(Event));

			CollisionEvent e;
			e.nx = -Event->nx;
			e.ny = -Event->ny;
			e.pGameObject = movingObj;

			Event->obj->OnCollisionEnter(e);
		}
	}
}


bool CollisionSystem::CheckOverlap(CGameObject* obj1, CGameObject* obj2)
{
	FRECT obj1_RECT = obj1->GetCollisionBox();
	FRECT obj2_RECT = obj2->GetCollisionBox();

	if (obj1_RECT.left >= obj2_RECT.right || obj2_RECT.left >= obj1_RECT.right)
		return false;

	if (obj1_RECT.top >= obj2_RECT.bottom || obj2_RECT.top >= obj1_RECT.bottom)
		return false;

	return true;
}

#pragma region SweptAABB functions

void CollisionSystem::SweptAABB(
    float ml, float mt, float mr, float mb, 
    float dx, float dy, 
    float sl, float st, float sr, float sb, 
    float& dt_Percent, float& nx, float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;



	float t_entry;
	float t_exit;

	dt_Percent = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	dt_Percent = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

LPCOLLISION CollisionSystem::SweptAABBEx(DynamicObject* movingObj, CGameObject* anotherObj, float dt)
{
	FRECT anotherObjRECT, movingObjRECT;
	D3DVECTOR movingObjVEL{ 0,0,0 }, anotherObjVEL{ 0,0,0 };

	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float dt_Percent, nx, ny;

	anotherObjRECT = anotherObj->GetCollisionBox();
	movingObjRECT = movingObj->GetCollisionBox();

	ml = movingObjRECT.left;
	mr = movingObjRECT.right;
	mt = movingObjRECT.top;
	mb = movingObjRECT.bottom;

	sl = anotherObjRECT.left;
	sr = anotherObjRECT.right;
	st = anotherObjRECT.top;
	sb = anotherObjRECT.bottom;

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;

	if (dynamic_cast<DynamicObject*>(anotherObj) != NULL)
	{
		anotherObjVEL = dynamic_cast<DynamicObject*>(anotherObj)->GetVelocity();
	}
	movingObjVEL = movingObj->GetVelocity();



	if (movingObjVEL.x == 0 && movingObjVEL.y == 0 ||
		anotherObjRECT == FRECT(0,0,0,0) ||
		movingObjRECT == FRECT(0,0,0,0))
	{
		Collision* e = new Collision(-1, 0, 0, 0, 0, NULL);
		return e;
	}

	float sdx = anotherObjVEL.x * dt;
	float sdy = anotherObjVEL.y * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = movingObjVEL.x*dt - sdx;
	float rdy = movingObjVEL.y*dt - sdy;

	SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		dt_Percent, nx, ny
	);

	Collision* e = new Collision(dt_Percent, nx, ny, rdx, rdy, anotherObj);
	return e;
}

#pragma endregion

std::pair<std::vector<LPCOLLISION>, std::vector<LPCOLLISION>> CollisionSystem::FilterCollisions(DynamicObject* movingObj, std::vector<LPCOLLISION> colList, float dt)
{
	LPCOLLISION minTxCol = nullptr;
	LPCOLLISION minTyCol = nullptr;

	std::pair<std::vector<LPCOLLISION>, std::vector<LPCOLLISION>> filteredCol;

	// check what minTxCol and minTyCol
	for (auto Event : colList)
	{
		if (Event->nx != 0 && (Event->nx / movingObj->GetVelocity().x) < 0)
		{
			if (minTxCol == nullptr) minTxCol = Event;
			else if (minTxCol->dt_Percent < Event->dt_Percent) minTxCol = Event;
		}

		if (Event->ny != 0 && (Event->ny / movingObj->GetVelocity().y) < 0)
		{
			if (minTyCol == nullptr) minTyCol = Event;
			else if (minTyCol->dt_Percent < Event->dt_Percent) minTyCol = Event;
		}
	}

	// find all collision that have the same minimum as minTxCol and minTyCol
	for (auto Event : colList)
	{
		if (Event->nx != 0 && (Event->nx / movingObj->GetVelocity().x) < 0)
		{
			if (Event->dt_Percent == minTxCol->dt_Percent)
				filteredCol.first.emplace_back(Event);
		}

		if (Event->ny != 0 && (Event->ny / movingObj->GetVelocity().y) < 0)
		{
			if (Event->dt_Percent == minTyCol->dt_Percent)
				filteredCol.second.emplace_back(Event);
		}
	}

	if (minTxCol == nullptr || minTyCol == nullptr)
	{
		return filteredCol;
	}

	D3DVECTOR oldVel = movingObj->GetVelocity();

	// x-axis
	if (dynamic_cast<RigidBody*>(minTyCol->obj) != nullptr)
	{
		float dty_Percent = minTyCol->dt_Percent;
		float ny_pushback = minTyCol->ny;

		movingObj->SetVelocity(oldVel.x, oldVel.y * dty_Percent + ny_pushback * 0.2);

		bool found = false;
		for (auto Event : filteredCol.first)
		{
			auto collision = SweptAABBEx(movingObj, Event->obj, dt);

			if (collision->dt_Percent > 0 || collision->dt_Percent <= 1)
			{
				found = true;
			}
		}

		movingObj->SetVelocity(oldVel.x, oldVel.y);

		if (!found)
		{
			filteredCol.first.clear();
			return filteredCol;
		}
	}

	// y-axis
	if (dynamic_cast<RigidBody*>(minTxCol->obj) != nullptr)
	{
		float dtx_Percent = minTxCol->dt_Percent;
		float nx_pushback = minTxCol->nx;

		movingObj->SetVelocity(oldVel.x * dtx_Percent + nx_pushback * 0.2, oldVel.y);

		bool found = false;
		for (auto Event : filteredCol.second)
		{
			auto collision = SweptAABBEx(movingObj, Event->obj, dt);

			if (collision->dt_Percent > 0 && collision->dt_Percent <= 1)
			{
				found = true;
			}
		}

		movingObj->SetVelocity(oldVel.x, oldVel.y);

		if (!found)
		{
			filteredCol.second.clear();
			return filteredCol;
		}
	}

	movingObj->SetVelocity(oldVel.x, oldVel.y);
	return filteredCol;
}

void CollisionSystem::FixPreOverlapped(DynamicObject* movingObj, std::vector<CGameObject*>* anotherObjs)
{
	if (dynamic_cast<Intangibility*>(movingObj) != NULL) return;

	std::vector<CGameObject*> overlapedObjs;
	std::vector<CGameObject*> colliableBricks;

	for (auto object : *anotherObjs)
	{
		if (dynamic_cast<ColliableBrick*>(object) != NULL)
			colliableBricks.emplace_back(object);
	}

	for (auto brick : colliableBricks)
	{
		if (CheckOverlap(movingObj, brick))
		{
			overlapedObjs.emplace_back(brick);
		}
	}

	std::vector<std::pair<float, float>> specialPushbackVector;

	bool specialCase = true;
	
	for (auto overlapedObj : overlapedObjs)
	{
		if (!CheckOverlap(movingObj, overlapedObj)) continue;

		auto oldMovingObjPos = movingObj->GetPosition();

		auto movingObjColBox = movingObj->GetCollisionBox();
		auto overlapedObjColBox = overlapedObj->GetCollisionBox();

		float deltaX = 0;
		float deltaY = 0;

		if (movingObjColBox.right > overlapedObjColBox.left && movingObjColBox.left < overlapedObjColBox.left &&
			oldMovingObjPos.x < overlapedObjColBox.left)
		{
			deltaX -= movingObjColBox.right - overlapedObjColBox.left + 0.5;
		}

		if (movingObjColBox.left < overlapedObjColBox.right && movingObjColBox.right > overlapedObjColBox.right &&
			oldMovingObjPos.x > overlapedObjColBox.right)
		{
			deltaX += overlapedObjColBox.right - movingObjColBox.left + 0.5;
		}

		if (movingObjColBox.bottom > overlapedObjColBox.top && movingObjColBox.top < overlapedObjColBox.top &&
			oldMovingObjPos.y < overlapedObjColBox.top)
		{
			deltaY -= movingObjColBox.bottom - overlapedObjColBox.top + 0.5;
		}

		if (movingObjColBox.top < overlapedObjColBox.bottom && movingObjColBox.bottom > overlapedObjColBox.bottom &&
			oldMovingObjPos.y > overlapedObjColBox.bottom)
		{
			deltaY += overlapedObjColBox.bottom - movingObjColBox.top + 0.5;
		}

		if (deltaX != 0 && deltaY != 0)
			specialPushbackVector.emplace_back(std::pair<float, float>(deltaX, deltaY));
		else
		{
			movingObj->SetPosition(oldMovingObjPos.x + deltaX, oldMovingObjPos.y + deltaY);
			specialCase = false;
		}
	}

	if (!specialCase || specialPushbackVector.size() == 0) return;

	for (auto pushbackVector : specialPushbackVector)
	{
		auto oldMovingObjPos = movingObj->GetPosition();

		if (pushbackVector.first < pushbackVector.second)
			pushbackVector.second = 0;
		else
			pushbackVector.first = 0;

		movingObj->SetPosition(oldMovingObjPos.x + pushbackVector.first, oldMovingObjPos.y + pushbackVector.second);
	}
}
