#include "Skull.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"

#define SKULL_BULLET_MOVE 27101;
#define SKULL_BULLET_EX 27201;

Skull_Bullet::Skull_Bullet()
{
	//state
	state = SKULL_BULLET_MOVE;
	//tick
	last = GetTickCount();
	//boolean
	ex = false;
	//animator
	animator->AddAnimation(27101);
	animator->AddAnimation(27201);
}

FRECT Skull_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 10 / 2;
	colRect.right = this->x + 10 / 2;
	colRect.top = this->y - 10 / 2;
	colRect.bottom = this->y + 10 / 2;
	return colRect;
}

void Skull_Bullet::OnCollisionEnter(CollisionEvent e)
{
	
}

void Skull_Bullet::Update(float dt)
{
	if (!ex)
	{
		CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		if (CollisionSystem::CheckOverlap(this, player))
		{
			// Set Dmg
			ex = true;
			last = GetTickCount();
		}
		DWORD now = GetTickCount();
		vy += 300 * dt;
		if (now - last > 2000)
		{
			ex = true;
			last = now;
			return;
		}
		float player_x = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
		if (player_x + 6 < this->x - 10 / 2)
		{
			vx = -55;
		}
		else if (player_x - 6 > this->x + 10 / 2)
		{
			vx = 55;
		}
	}
}

void Skull_Bullet::Explode()
{
	state = SKULL_BULLET_EX;
	vx = 0;
	vy = 0;
	DWORD now = GetTickCount();
	if(now - last > 600)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Skull_Bullet::Render()
{
	if (ex)
	{
		Explode();
	}
	animator->Draw(state, x, y, false);
}