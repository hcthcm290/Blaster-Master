#include "Teleporter.h"
#include "Time.h"
#include "InteriorScene.h"
#include "CollisionSystem.h"
#include "Debug.h"
#include "Explosive.h"
#include "Rock.h"

#define TELEPORTER_TELE 21001
#define TELEPORTER_START 21002
#define TELEPORTER_CD 21003

Teleporter::Teleporter(FRECT zone)
{
	this->zone = zone;
	//set HP
	HP = 40;
	//animator
	animator->AddAnimation(21001);
	animator->AddAnimation(21002);
	animator->AddAnimation(21003);
	state = TELEPORTER_CD;
	start_tele = GetTickCount();
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
	if (cooldown <= 0)
	{
		if (state == TELEPORTER_CD)
		{
			state = TELEPORTER_START;
			start_tele = GetTickCount();
			invincible = false;
		}
		else
		{
			if (GetTickCount() - start_tele > 400)
			{
				state = TELEPORTER_TELE;
				if (step > 0)
				{
					switch (step)
					{
					case 1:
					{
						sx = 0;
						sy = 0;
						if (abs(player_x - this->x) > abs(player_y - this->y))
						{
							if (player_x > this->x)
								sx = 50;
							else
								sx = -50;
						}
						else
						{
							if (player_y > this->y)
								sy = 50;
							else
								sy = -50;
						}
						break;
					}
					case 2:
						sx = ox * 50; sy = 0; break;
					case 3:
						sy = oy * 50; sx = 0;  break;
					case 4:
						sx = ox * -50; sy = 0; break;
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
						sy = oy * -25; sx = 0; break;
					}
					}
					step--;
					cooldown = 500;
				}
				else
				{
					sx = 0;
					sy = 0;
					cooldown = 2000;
					step = 5;
					state = TELEPORTER_CD;
					invincible = true;
				}
			}
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
	if (x < zone.left)
	{
		x = zone.left;
	}
	if (x > zone.right)
	{
		x = zone.right;
	}
	if (y < zone.top)
	{
		y = zone.top;
	}
	if (y > zone.bottom)
	{
		y = zone.bottom;
	}
	for (auto object : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
	{
		if (dynamic_cast<Rock*>(object) != nullptr)
		{
			if (CollisionSystem::CheckOverlap(object, this))
			{
				x -= sx / 50 * 8;
				y -= sy / 50 * 8;
			}
		}
	}
}

void Teleporter::Render()
{
	animator->Draw(state, x, y, false);
}
void Teleporter::TakeDamage(int dmg)
{
	if (invincible)
		return;
	this->HP -= dmg;
	startTakeDamage = GetTickCount();
	last_blink = GetTickCount();
	inv = 1;
	if (HP < 0)
	{
		HP = 0;
	}
	if (HP == 0)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
}