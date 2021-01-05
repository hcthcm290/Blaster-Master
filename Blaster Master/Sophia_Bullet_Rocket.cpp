#include "Sophia_Bullet_Rocket.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"
#include "Explosive.h"
#include "Camera.h"
#include "Enemy.h"
#include "VisionBox.h"
#include "Sound.h"
#define SOPHIA_BULLET_H_LR 29601

int Sophia_Bullet_Rocket::count = 0;

Sophia_Bullet_Rocket::Sophia_Bullet_Rocket()
{
	flipX = true;
	//state
	state = SOPHIA_BULLET_H_LR;
	//tick
	last_anim = GetTickCount();
	last = GetTickCount();
	//boolean
	ex = false;
	up = false;
	down = false;
	//animator
	animator->AddAnimation(29601);
	animator->AddAnimation(29602);
}

Sophia_Bullet_Rocket::Sophia_Bullet_Rocket(bool flipX, int num)
{
	this->num = num;
	animator->AddAnimation(29601);
	animator->AddAnimation(29602);
	this->flipX = flipX;
	state = SOPHIA_BULLET_H_LR;
	//tick
	last = GetTickCount();
	//boolean
	ex = false;
	count++;
	vx = 0;
	vy = 0;
	Sound::getInstance()->play("3Missile",false,1);
}

FRECT Sophia_Bullet_Rocket::GetCollisionBox()
{
	FRECT colRect;
	int width, height;
	width = 6;
	height = 7;
	colRect.left = this->x - width / 2;
	colRect.right = this->x + width / 2;
	colRect.top = this->y - height / 2;
	colRect.bottom = this->y + height / 2;
	return colRect;
}

void Sophia_Bullet_Rocket::OnCollisionEnter(CollisionEvent e)
{
	//CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//e.pGameObject lay va cham goi ham tinh sat thuong
	if (dynamic_cast<Enemy*>(e.pGameObject) != nullptr)
	{
		dynamic_cast<DynamicObject*>(e.pGameObject)->TakeDamage(100);
		//ex = true;
		RemoveBullet();
	}
}

void Sophia_Bullet_Rocket::Update(float dt)
{
	if (!ex)
	{
		if (flipX)
			vx = 30 + 150*time;
		else
			vx = -30 - 150*time;
		if (num == 1)
		{
			vy = 0;
		}
		if (num == 2)
		{
			vy = 36 * ((float)1 / (time + (float)2 / 3));
		}
		if (num == 3)
		{
			vy = -36 * ((float)1 / (time + (float)2 / 3));
		}
		time += dt;
	}
	CGameObject* iBullet = new Sophia_Bullet_Rocket();
	iBullet->SetPosition(x + vx * dt, y + vy * dt);
	FRECT camera = Camera::GetInstance()->GetCollisionBox();
	FRECT me = iBullet->GetCollisionBox();
	if (me.right <= camera.left || me.left >= camera.right || me.bottom <= camera.top || me.top >= camera.bottom)
		RemoveBullet();
	delete iBullet;
}

void Sophia_Bullet_Rocket::Explode()
{
	auto explode = new Explosive();
	if (flipX)
	{
		explode->SetPosition(x + 4, y);
	}
	else
	{
		explode->SetPosition(x - 4, y);
	}
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
	RemoveBullet();
}

void Sophia_Bullet_Rocket::RemoveBullet()
{
	count--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}


void Sophia_Bullet_Rocket::Render()
{
	if (ex)
	{
		Explode();
	}
	animator->Draw(state, x, y, flipX);
	float sx = x, sy = y;
	if (flipX)
	{
		sx -= 7;
	}
	else
	{
		sx += 7;
	}
	DWORD now = GetTickCount();
	if (now - last_anim > 100)
	{
		last_anim = now;
		if (temp == 1)
		{
			temp = 2;
		}
		else
		{
			temp = 1;
		}
	}
	if (temp == 1)
	{
		animator->Draw(state + 5, sx, sy, flipX);
	}
	else
	{
		animator->Draw(state + 1, sx, sy, flipX);
	}

}