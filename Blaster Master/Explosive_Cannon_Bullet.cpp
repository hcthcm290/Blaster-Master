
#include "Explosive_Cannon_Bullet.h"
#include "InteriorScene.h"
#include "Debug.h"

Explosive_Cannon_Bullet::Explosive_Cannon_Bullet()
{
	animator->AddAnimation(27201);
	last = GetTickCount();
}

FRECT Explosive_Cannon_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x + 5;
	colRect.right = x - 5;
	colRect.top = y + 5;
	colRect.bottom = y - 5;
	return colRect;
}

void Explosive_Cannon_Bullet::Update(float dt)
{
	DWORD now = GetTickCount();
	if (now - last > 110)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Explosive_Cannon_Bullet::Render()
{
	animator->Draw(27201, x, y, false);
}