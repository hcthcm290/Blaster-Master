#include "Teleporter.h"
#include "Time.h"
#include "InteriorScene.h"
#include "CollisionSystem.h"
#include "Debug.h"

Teleporter::Teleporter()
{
	//set HP
	HP = 40;
	//animator
	animator->AddAnimation(21001);
}
FRECT Teleporter::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 26 / 2;
	colRect.right = this->x + 26 / 2;
	colRect.top = this->y - 33 / 2;
	colRect.bottom = this->y + 33 / 2;
	return colRect;
}

void Teleporter::OnCollisionEnter(CollisionEvent e)
{
	
}
void Teleporter::Update(float dt)
{
	CGameObject* player = dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(8);
	}
	float player_x = player->GetPosition().x;
	float player_y = player->GetPosition().y;
	cooldown -= (int) (dt * 1000);
	DebugOut(L"%d\n",cooldown);
	if (cooldown <= 0)
	{
		if (step > 0)
		{
			switch (step)
			{
			case 1:
			{
				sx = 0; sy = 0;
				break;
			}
			case 2:
				sx = ox*50; sy = 0; break;
			case 3:
				sy = oy*50; sx = 0;  break;
			case 4:
				sx = ox*-50; sy = 0; break;
			case 5:
			{
				ox = 1;
				oy = 1;
				if (player_x > this->x)
				{
					ox = -1;
				}
				if (player_y > this->y)
				{
					oy = -1;
				}
				sy = oy*-25; sx = 0; break;
			}
			}
			step--;
			cooldown = 500;
		}
		else
		{
			sx = 0;
			sy = 0;
			cooldown = 3000;
			step = 5;
		}
	}
	else
	{
		sx = 0;
		sy = 0;
	}
	srand((int)time(0));
	sx *= rand() % 2 + 1;
	sy *= rand() % 2 + 1;
	x += sx;
	y += sy;
}

void Teleporter::Render()
{
	animator->Draw(21001, x, y, false);
}