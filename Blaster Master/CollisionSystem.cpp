#include "CollisionSystem.h"
#include <algorithm>
#include "RigidBody.h"
#include "Debug.h"
#include "Orb.h"
#include "Jumper.h"

#include "Jason.h"
#include "Debug.h"

void CollisionSystem::DoCollision(DynamicObject* movingObj, std::vector<CGameObject*>* anotherObjs, float dt)
{
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

	auto filteredCol = FilterCollisions(collisions);

	float dtx_Percent = 1;
	float dty_Percent = 1;
	float nx_pushback = 0;
	float ny_pushback = 0;

	// deal with collision object at x axis
	if (filteredCol.first != nullptr) 
	{
		// only push moving obj back if both of them have rigidbody
		if (dynamic_cast<RigidBody*>(movingObj) != NULL && dynamic_cast<RigidBody*>(filteredCol.first->obj) != NULL)
		{
			dtx_Percent = filteredCol.first->dt_Percent;
			nx_pushback = filteredCol.first->nx;
		}
	}

	// deal with collision object at y axis
	if (filteredCol.second != nullptr)
	{
		// only push moving obj back if both of them have rigidbody
		if (dynamic_cast<RigidBody*>(movingObj) != NULL && dynamic_cast<RigidBody*>(filteredCol.second->obj) != NULL)
		{
			dty_Percent = filteredCol.second->dt_Percent;
			ny_pushback = filteredCol.second->ny;
		}
	}

	auto movingObjVEL = movingObj->GetVelocity();
	auto movingObjPOS = movingObj->GetPosition();

	auto vxx = movingObjVEL.y * dty_Percent + ny_pushback * 0.5 / dt;

	//movingObj->SetPosition(movingObjPOS.x + nx_pushback * 0.2, movingObjPOS.y + ny_pushback * 0.2);
	movingObj->SetPosition(movingObjPOS.x + nx_pushback * 2, movingObjPOS.y + ny_pushback * 2);
	movingObj->SetVelocity(movingObjVEL.x * dtx_Percent, movingObjVEL.y * dty_Percent);

	if (filteredCol.first != NULL)
	{
		movingObj->OnCollisionEnter(CollisionEvent(filteredCol.first));

		CollisionEvent e;
		e.nx = -filteredCol.first->nx;
		e.ny = -filteredCol.first->ny; 
		e.pGameObject = filteredCol.first->obj;

		filteredCol.first->obj->OnCollisionEnter(e);
	}
	if (filteredCol.second != NULL)
	{
		// TODO: SEND COLLISIONEVENT TO BOTH GAMEOBJECT //
		movingObj->OnCollisionEnter(CollisionEvent(filteredCol.second));

		CollisionEvent e;
		e.nx = -filteredCol.second->nx;
		e.ny = -filteredCol.second->ny;
		e.pGameObject = filteredCol.second->obj;

		filteredCol.second->obj->OnCollisionEnter(e);
		//////////////////////////////////////////////////
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

std::pair<LPCOLLISION, LPCOLLISION> CollisionSystem::FilterCollisions(std::vector<LPCOLLISION> colList)
{
	LPCOLLISION minTxCol = nullptr;
	LPCOLLISION minTyCol = nullptr;

	for (auto Event : colList)
	{
		if (Event->nx != 0)
		{
			if (minTxCol == nullptr) minTxCol = Event;
			else if (minTxCol->dt_Percent < Event->dt_Percent) minTxCol = Event;
		}

		if (Event->ny != 0)
		{
			if (minTyCol == nullptr) minTyCol = Event;
			else if (minTyCol->dt_Percent < Event->dt_Percent) minTyCol = Event;
		}
	}

	return std::pair<LPCOLLISION, LPCOLLISION>(minTxCol, minTyCol);
}
