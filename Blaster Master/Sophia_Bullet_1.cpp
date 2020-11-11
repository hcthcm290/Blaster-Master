#include "Sophia_Bullet_1.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Sophia_Bullet_1.h"
#include "Skull_Bullet.h"
#include "Explosive.h"
#include "Camera.h"

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
	//CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//e.pGameObject lay va cham goi ham tinh sat thuong
	if (dynamic_cast<Bullet*>(e.pGameObject) != nullptr)
	{
		DebugOut(L"No damage\n");
	}
	else
	{
		ex = true;
	}
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
		CGameObject* iBullet = new Sophia_Bullet_1();
		iBullet->SetPosition(x + vx * dt, y + vy * dt);
		FRECT camera = Camera::GetInstance()->GetCollisionBox();
		FRECT me = iBullet->GetCollisionBox();
		if (me.right <= camera.left || me.left >= camera.right || me.bottom <= camera.top)
			RemoveBullet();
	}
}

void Sophia_Bullet_1::Explode()
{
	auto explode = new Explosive();
	if (up)
	{
		explode->SetPosition(x, y - 4);
	}
	else
	{
		if (flipX)
		{
			explode->SetPosition(x + 4, y);
		}
		else
		{
			explode->SetPosition(x - 4, y);
		}
	}
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
	RemoveBullet();
}
void Sophia_Bullet_1::RemoveBullet()
{
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