#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "PlayerBullet.h"
#include "Intangibility.h"

class Sophia_Bullet_Thunder :
	public DynamicObject, public PlayerBullet, public Intangibility
{
protected:
	DWORD last;
	int state;
	//bool ex;
	bool flipX;
	DWORD last_anim;
	int temp;
	int sy;
	Sophia_Bullet_Thunder* lastThunder;
	bool isLastThunder = true;

	static constexpr D3DCOLOR Color[] = {
	D3DCOLOR_ARGB(255,255,255,255),
	D3DCOLOR_ARGB(128,124,124,124),
	D3DCOLOR_ARGB(255,247,164,143),
	D3DCOLOR_ARGB(255,255,255,0),
	D3DCOLOR_ARGB(255,0,50,200),
	};
	int currentColor = 4;
	DWORD lastBlinkTime;
public:
	static int count;
	static bool ex;
	Sophia_Bullet_Thunder();
	Sophia_Bullet_Thunder(bool flipX, Sophia_Bullet_Thunder* lastThunder);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Explode();
	void Update(float dt);
	void Render();
	void RemoveBullet();
};

