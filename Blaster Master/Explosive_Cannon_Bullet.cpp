
#include "Explosive_Cannon_Bullet.h"
#include "InteriorScene.h"
#include "Debug.h"
#include "SoundManager.h"

Explosive_Cannon_Bullet::Explosive_Cannon_Bullet()
{
	animator->AddAnimation(211201);
	last = GetTickCount();
	SoundManager::GetInstance()->PlaySoundW("Explosive_bullet.wav");
}

FRECT Explosive_Cannon_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x + 8;
	colRect.right = x - 8;
	colRect.top = y + 8;
	colRect.bottom = y - 8;
	return colRect;
}

void Explosive_Cannon_Bullet::Update(float dt)
{
	DWORD now = GetTickCount();
	if (now - last > 160)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Explosive_Cannon_Bullet::Render()
{
	animator->Draw(211201, x, y, false);
}