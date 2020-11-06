#include "Skull.h"
#include "PlayScene.h"
Skull::Skull()
{
	//boolean
	sleep = false;
	//tick
	last = 0;
	//animator
	animator->AddAnimation(20701);
	animator->AddAnimation(20702);
}

FRECT Skull::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 20 / 2;
	colRect.right = this->x + 20 / 2;
	colRect.top = this->y - 22 / 2;
	colRect.bottom = this->y + 22 / 2;
	return colRect;
}

void Skull::OnCollisionEnter(CollisionEvent e)
{

}

void Skull::Update(float dt)
{
	DWORD now = GetTickCount();
	float player_x = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	if (sleep)
	{
		if (now - last > 100)
		{
			vx = 0;
			vy = -50;
		}
		if (player_x == this->x)
			sleep = false;
	}
	else
	{
		if (player_x < this->x - 20 / 2)
			vx = -500;
		else if (player_x > this->x + 20 / 2)
			vx = 500;
		else
		{
			//drop the bullet here
			sleep = true;
			last = now;
		}
	}
}

void Skull::Render()
{
	animator->Draw(20701, x, y, false);
}