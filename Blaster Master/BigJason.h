#pragma once
#include "DynamicObject.h"
#include "Playable.h"
#include "Jason_BulletManager_Interior.h"

class BigJason : public DynamicObject, public Playable
{
protected:
	//
	int invincible = 0;
	//
	bool flipX = true;
	int state;
	bool isSide = true;
	bool isUpDown = false;

	static constexpr D3DCOLOR damageColor[] = {
	D3DCOLOR_ARGB(255,255,255,255),
	D3DCOLOR_ARGB(255,148,247,207),
	D3DCOLOR_ARGB(128,124,124,124),
	D3DCOLOR_ARGB(255,247,164,143),
	D3DCOLOR_ARGB(128,124,124,124),
	};
	int currentColor = 0;
	DWORD lastDamageTime;
	DWORD die;
private:
	Jason_BulletManager* bulletManager = new Jason_BulletManager();
	bool keypress = true;

	DWORD lastGrenadeTime;
public:
	BigJason();
	void Update(float dt);
	void Render();
	virtual FRECT GetCollisionBox();
	bool IsInvulnerable();
	void TakeDamage(int dmg);
	float GetEnterGateSpeed();
	int getGunPower() { return this->bulletManager->GetLevel(); }
	void NotifySwitchSceneOut();
	bool IsDead();
	void ChangeGunLevel(int levelchange);
	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }
};
