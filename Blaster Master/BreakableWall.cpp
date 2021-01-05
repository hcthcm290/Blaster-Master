#include "BreakableWall.h"
#include "PlayScene.h"
BreakableWall::BreakableWall()
{
	//set HP
	HP = 40;
	SetSpriteID(1);
}

FRECT BreakableWall::GetCollisionBox()
{
	FRECT colRect;

	colRect.left = x - 16 / 2;
	colRect.right = x + 16 / 2;
	colRect.top = y - 16 / 2;
	colRect.bottom = y + 16 / 2;
	return colRect;
}

void BreakableWall::OnCollisionEnter(CollisionEvent e)
{
}

void BreakableWall::Update(float dt)
{

}

void BreakableWall::TakeDamage(int dmg)
{
	this->HP -= dmg;
	if (HP < 0)
	{
		HP = 0;
	}
	if (HP == 0)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
}