#include "Skull.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"
#include "Explosive.h"

#define SKULL_BULLET_MOVE 27101;

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
		}
		DWORD now = GetTickCount();
		vy += 300 * dt;
		if (now - last > 2000)
		{
			ex = true;
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
	auto explode = new Explosive();
	explode->SetPosition(x, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);

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