#include "Sophia_Bullet_1.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Sophia_Bullet_1.h"
#include "Explosive.h"

#define SOPHIA_BULLET_LR 29701
#define SOPHIA_BULLET_UP 29702

int Sophia_Bullet_1::count = 0;

Sophia_Bullet_1::Sophia_Bullet_1()
{
	flipX = true;
	//state
	state = SOPHIA_BULLET_LR;
	//tick
	last = GetTickCount();
	//boolean
	ex = false;
	up = false;
	//animator
	animator->AddAnimation(29701);
}

Sophia_Bullet_1::Sophia_Bullet_1(bool up, bool flipX)
{
	this->flipX = flipX;
	if (up)
	{
		this->up = up;
		state = SOPHIA_BULLET_UP;
		animator->AddAnimation(29702);
	}
	else
	{
		this->up = up;
		state = SOPHIA_BULLET_LR;
		animator->AddAnimation(29701);
	}
	//tick
	last = GetTickCount();
	//boolean
	ex = false;
	count++;
	DebugOut(L"%d\n", count);
}

FRECT Sophia_Bullet_1::GetCollisionBox()
{
	FRECT colRect;
	int width, height;
	if (state == SOPHIA_BULLET_LR)
	{
		width = 25;
		height = 7;
	}
	else
	{
		width = 7;
		height = 25;
	}
	colRect.left = this->x - width / 2;
	colRect.right = this->x + width / 2;
	colRect.top = this->y - height / 2;
	colRect.bottom = this->y + height / 2;
	return colRect;
}

void Sophia_Bullet_1::OnCollisionEnter(CollisionEvent e)
{
	//e.pGameObject lay va cham goi ham tinh sat thuong
	ex = true;
}

void Sophia_Bullet_1::Update(float dt)
{
	if (!ex)
	{
		DWORD now = GetTickCount();
		if (now - last > 5000)
		{
			count--;
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		}

		if (up)
		{
			vy = -150;
			vx = 0;
		}
		else
		{

			if (flipX)
			{
				vx = 150;
				vy = 0;
			}
			else
			{
				vx = -150;
				vy = 0;
			}
		}
	}
}

void Sophia_Bullet_1::Explode()
{
	auto explode = new Explosive();
	explode->SetPosition(x, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
	count--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Sophia_Bullet_1::Render()
{
	if (ex)
	{
		Explode();
	}
	animator->Draw(state, x, y, flipX);
}