#include "Sophia_Bullet_Homing.h"
#include "PlayScene.h"
#include "Debug.h"
#include "Sophia.h"
#include "Skull_Bullet.h"
#include "Explosive.h"
#include "Camera.h"
#include "Enemy.h"
#include "VisionBox.h"

#define SOPHIA_BULLET_H_LR 29601
#define SOPHIA_BULLET_H_UP 29603

int Sophia_Bullet_Homing::count = 0;

Sophia_Bullet_Homing::Sophia_Bullet_Homing()
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
	animator->AddAnimation(29603);
	animator->AddAnimation(29604);
}

Sophia_Bullet_Homing::Sophia_Bullet_Homing(bool flipX)
{
	animator->AddAnimation(29601);
	animator->AddAnimation(29602);
	animator->AddAnimation(29603);
	animator->AddAnimation(29604);
	this->flipX = flipX;
	state = SOPHIA_BULLET_H_LR;
	//tick
	last = GetTickCount();
	//boolean
	ex = false;
	count++;
}

FRECT Sophia_Bullet_Homing::GetCollisionBox()
{
	FRECT colRect;
	int width, height;
	if (state == SOPHIA_BULLET_H_LR)
	{
		width = 6;
		height = 7;
	}
	else
	{
		width = 7;
		height = 6;
	}
	colRect.left = this->x - width / 2;
	colRect.right = this->x + width / 2;
	colRect.top = this->y - height / 2;
	colRect.bottom = this->y + height / 2;
	return colRect;
}

void Sophia_Bullet_Homing::OnCollisionEnter(CollisionEvent e)
{
	//CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//e.pGameObject lay va cham goi ham tinh sat thuong
	if (dynamic_cast<Enemy*>(e.pGameObject) != nullptr)
	{
		ex = true;
	}
}

void Sophia_Bullet_Homing::Update(float dt)
{
	if (!ex)
	{
		if (target == nullptr)
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
				if (down)
				{
					vy = 150;
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

			CGameObject* vision = new VisionBox(x - 20, x + 20, y - 20, y + 20);

			for (auto enemy : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->onSCeneObjs)
			{
				if (dynamic_cast<Enemy*>(enemy) != nullptr)
				{
					if (CollisionSystem::CheckOverlap(enemy, vision))
					{
						target = dynamic_cast<DynamicObject*>(enemy);
						break;
					}
				}
			}
			delete vision;
		}
		else
		{
			state = SOPHIA_BULLET_H_LR;
			if (this->GetCollisionBox().left >= target->GetCollisionBox().right)
			{
				vx = -150;
				vy = 0;
				flipX = false;
			}
			else
			{
				if (this->GetCollisionBox().right <= target->GetCollisionBox().left)
				{
					vx = 150;
					vy = 0;
					flipX = true;
				}
				else
				{
					state = SOPHIA_BULLET_H_UP;
					if (this->GetCollisionBox().top <= target->GetCollisionBox().bottom)
					{
						vx = 0;
						vy = 150;
					}
					else
					{
						if (this->GetCollisionBox().bottom >= target->GetCollisionBox().top)
						{
							vx = 0;
							vy = -150;
						}
					}
				}
			}
		}
	}
	CGameObject* iBullet = new Sophia_Bullet_Homing();
	iBullet->SetPosition(x + vx * dt, y + vy * dt);
	FRECT camera = Camera::GetInstance()->GetCollisionBox();
	FRECT me = iBullet->GetCollisionBox();
	if (me.right <= camera.left || me.left >= camera.right || me.bottom <= camera.top || me.top >= camera.bottom)
		RemoveBullet();
	delete iBullet;
}

void Sophia_Bullet_Homing::Explode()
{
	auto explode = new Explosive();
	if (up)
	{
		explode->SetPosition(x, y - 4);
	}
	else
	{
		if (down)
		{
			explode->SetPosition(x, y + 4);
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
	}
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
	RemoveBullet();
}

void Sophia_Bullet_Homing::RemoveBullet()
{
	count--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}


void Sophia_Bullet_Homing::Render()
{
	if (ex)
	{
		Explode();
	}
	animator->Draw(state, x, y, flipX);
	float sx = x, sy = y;
	if (up)
	{
		sy += 7;
	}
	else
	{
		if (down)
		{
			sy -= 7;
		}
		else
		{
			if (flipX)
			{
				sx -= 7;
			}
			else
			{
				sx += 7;

			}
		}
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