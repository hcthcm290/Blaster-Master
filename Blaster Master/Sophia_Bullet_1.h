#pragma once
#include "DynamicObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "Bullet.h"
#include "PlayerBullet.h"

class Sophia_Bullet_1 :
    public DynamicObject, public PlayerBullet
{
protected:
	DWORD last;
	int state;
	bool ex;
	bool flipX;
	bool up;
	bool isHit = false;
	bool explosionOverlap = false;

	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(255,255,0,0),
	D3DCOLOR_ARGB(255,255,255,255),
	D3DCOLOR_ARGB(255,255,255,0),
	D3DCOLOR_ARGB(255,0,255,0),
	D3DCOLOR_ARGB(255,255,255,255),
	D3DCOLOR_ARGB(255,0,0,255),
	};

public:
	static int count;
	bool upgraded = false;
	Sophia_Bullet_1();
	Sophia_Bullet_1(bool up, bool flipX);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void OnOverlap(CGameObject* obj);
	void Explode();
	void Update(float dt);
	void Render();
	void RemoveBullet();

	bool isWallBreak() { return upgraded; }
};

