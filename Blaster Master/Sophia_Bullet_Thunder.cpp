#include "Sophia_Bullet_Thunder.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"
#include "Explosive.h"
#include "Camera.h"
#include "Enemy.h"
#include "VisionBox.h"
#include "Sound.h"

#define SOPHIA_BULLET_T_1 29501
#define SOPHIA_BULLET_T_2 29502

int Sophia_Bullet_Thunder::count = 0;
bool Sophia_Bullet_Thunder::ex = false;

Sophia_Bullet_Thunder::Sophia_Bullet_Thunder()
{
	flipX = true;
	//state
	state = SOPHIA_BULLET_T_1;
	//tick
	last_anim = GetTickCount();
	last = GetTickCount();
	//boolean
	//ex = false;
	sy = 0;
	//animator
	animator->AddAnimation(29501);
	animator->AddAnimation(29502);
	Sound::getInstance()->play("Thunder",false,1);
}

Sophia_Bullet_Thunder::Sophia_Bullet_Thunder(bool flipX, Sophia_Bullet_Thunder* lastThunder)
{
	count++;
	this->flipX = flipX;
	this->lastThunder = lastThunder;
	
	//state
	state = SOPHIA_BULLET_T_1;
	if (lastThunder != nullptr)
	{
		currentColor = lastThunder->currentColor;
		state = lastThunder->state + 1;
		if (state % 2 == 1)
		{
			state = SOPHIA_BULLET_T_1;
		}
	}
	//tick
	last_anim = GetTickCount();
	last = GetTickCount();
	lastBlinkTime = GetTickCount();
	//boolean
	ex = false;
	sy = 0;
	//animator
	animator->AddAnimation(29501);
	animator->AddAnimation(29502);
}

FRECT Sophia_Bullet_Thunder::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 15 / 2;
	colRect.right = this->x + 15 / 2;
	colRect.top = this->y - 16 / 2;
	colRect.bottom = this->y + 16 / 2;
	return colRect;
}

void Sophia_Bullet_Thunder::OnCollisionEnter(CollisionEvent e)
{
	//CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//e.pGameObject lay va cham goi ham tinh sat thuong
	if (dynamic_cast<Enemy*>(e.pGameObject) != nullptr)
	{
		dynamic_cast<DynamicObject*>(e.pGameObject)->TakeDamage(60);
		ex = true;
	}
}

void Sophia_Bullet_Thunder::Update(float dt)
{
	if (!ex)
	{
		for (auto enemy : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
		{
			if (dynamic_cast<Enemy*>(enemy) != nullptr && CollisionSystem::CheckOverlap(this, enemy))
			{
				dynamic_cast<DynamicObject*>(enemy)->TakeDamage(60);
				ex = true;
			}
		}
		CGameObject* iBullet = new Sophia_Bullet_Thunder();
		iBullet->SetPosition(x, y + 20);
		FRECT camera = Camera::GetInstance()->GetCollisionBox();
		FRECT me = iBullet->GetCollisionBox();
		if (me.right <= camera.left || me.left >= camera.right || me.bottom <= camera.top || me.top >= camera.bottom)
		{
			ex = true;
			//RemoveBullet();
		}
		delete iBullet;
		if (GetTickCount() - last > 50)
		{
			if (count <= 6)
			{
				if (isLastThunder)
				{
					int temp;
					auto bullet = new Sophia_Bullet_Thunder(true, this);
					srand((int)time(0));
					if ((rand() % 2) == 0)
					{
						temp = 4;
					}
					else
					{
						temp = -4;
					}
					bullet->SetPosition(x + temp, y + 20);
					isLastThunder = false;
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
				}
			}
			else
			{
				ex = true;
			}
		}
		if (GetTickCount() - lastBlinkTime > 100)
		{
			switch (currentColor)
			{
			case 4: currentColor = 3; break;
			case 3: currentColor = 2; break;
			case 2: currentColor = 1; break;
			case 1: currentColor = 0; break;
			case 0: currentColor = 4; break;
			}
			//currentColor = 5 - currentColor;
			lastBlinkTime = GetTickCount();
		}
	}
}

void Sophia_Bullet_Thunder::Explode()
{
	auto explode = new Explosive();
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
	RemoveBullet();
}

void Sophia_Bullet_Thunder::RemoveBullet()
{
	count--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}


void Sophia_Bullet_Thunder::Render()
{
	if (ex)
	{
		DebugOut(L"%d\n", count);
		Explode();
	}
	animator->Draw(state, x, y, flipX, 0, Color[currentColor]);

}