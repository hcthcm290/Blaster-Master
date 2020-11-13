#include "Skull.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"

#define SKULL_MOVE 20701
#define SKULL_SHOOT 20702
#define SKULL_DIE 27201

Skull::Skull()
{
	//boolean
	flipX = false;
	sleep = false;
	//tick
	last = 0;
	//state
	state = SKULL_MOVE;
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
	colRect.bottom = this->y + 14 / 2;
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
		if (state != SKULL_MOVE)
		{
			if (now - last > 50)
			{
				vx = 0;
				vy = -500;
			}
			if (now - last > 500)
			{
				state = SKULL_MOVE;
				last = now;
			}
		}
		else
		{
			if (player_x > this->x - 8 && player_x < this->x + 8)
			{
				sleep = false;
				if (flipX)
				{
					vx = 500;
				}
				else
				{
					vx = -500;
				}
			}
		}
	}
	else
	{
		if (player_x < this->x - 20 / 2)
		{
			vx = -500;
			flipX = false;
		}
		else if (player_x > this->x + 20 / 2)
		{
			vx = 500;
			flipX = true;
		}
		else
		{
			//drop the bullet here
			if (state != SKULL_SHOOT)
			{
				flipX = !flipX;
				state = SKULL_SHOOT;
				DropBomb();
			}
			sleep = true;
			last = now;
		}
	}
}

void Skull::Render()
{
	animator->Draw(state, x, y, flipX);
}

void Skull::DropBomb()
{
	auto bomb = new Skull_Bullet();
	bomb->SetPosition(x, y);

	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bomb);
}