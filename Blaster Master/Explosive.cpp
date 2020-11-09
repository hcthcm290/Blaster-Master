#include "Explosive.h"
#include "PlayScene.h"
#include "Debug.h"

Explosive::Explosive()
{
	animator->AddAnimation(27201);
	last = GetTickCount();
}

FRECT Explosive::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 1;
	colRect.right = x + 1;
	colRect.top = y - 1;
	colRect.bottom = y + 1;
	return colRect;
}

void Explosive::Update(float dt)
{
	DWORD now = GetTickCount();
	if (now - last > 600)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Explosive::Render()
{
	animator->Draw(27201, x, y, false);
}