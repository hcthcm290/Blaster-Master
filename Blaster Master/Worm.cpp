#include "Worm.h"
#include "PlayScene.h"
#include "Debug.h"
#include "ColliableBrick.h"
#include "Explosive.h"
#include "Sophia.h"
#include "Sound.h"

Worm::Worm()
{
	//set HP
	HP = 40;
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
			vy = -100;
			Sound::getInstance()->play("BigObjectJump",false,1);
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
	SoundCount += dt;
	if (SoundCount >= 1)
	{
		Sound::getInstance()->play("WormMoving_CannonShot_TeleporterShot",false,1);
		SoundCount = 0;
	}
	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(3);
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
	if (inv != -1) {
		animator->Draw(20502, x, y, flipX, 0, Color[inv]);
		if (GetTickCount64() - last_blink >= 50) {
			if (GetTickCount64() > startTakeDamage + 150)
			{
				inv = -1;
			}
			else
			{
				last_blink = GetTickCount64();
				switch (inv)
				{
				case 1: inv = 0; break;
				case 0: inv = 1; break;
				}
			}
		}
	}
	else
	{
		animator->Draw(20502, x, y, flipX);
	}
}