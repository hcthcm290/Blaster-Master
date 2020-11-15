#include "Worm.h"
#include "PlayScene.h"
#include "Debug.h"
#include "ColliableBrick.h"
#include "Explosive.h"
#include "Sophia.h"

Worm::Worm()
{
	//set HP
	HP = 20;
	//boolean
	flipX = false;
	//animator
	animator->AddAnimation(20502);
}

FRECT Worm::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 18 / 2;
	colRect.right = this->x + 18 / 2;
	colRect.top = this->y - 4 / 2;
	colRect.bottom = this->y + 6 / 2;
	return colRect;
}

void Worm::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != nullptr)
	{
		if (e.nx != 0 && onTheGround)
		{
			vy = -50;
			onTheGround = false;
			if (flipX)
			{
				vx = 50;
			}
			else
			{
				vx = -50;
			}
		}
		if (e.ny < 0)
		{
			//vy = 0;
			onTheGround = true;
		}
	}
}

void Worm::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(8);
	}
	vy += 300 * dt;
	float player_x = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	if (player_x < this->x - 18 / 2)
	{
		vx = -50;
		flipX = false;
	}
	else if (player_x > this->x + 18 / 2)
	{
		vx = 50;
		flipX = true;
	}
}

void Worm::Render()
{
	animator->Draw(20502, x, y, flipX);
}