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

Sophia_Bullet_Homing::Sophia_Bullet_Homing(bool flipX, DynamicObject* target)
{
	this->target = target;
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
	vx = 0;
	vy = 0;
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
		state = SOPHIA_BULLET_H_LR;
		up = false;
		down = false;
		if (this->GetCollisionBox().left >= target->GetCollisionBox().right)
		{
			if (vx > -100)
			{
				vx += dt * -100 * 4;
			}
			else
			{
				vx = -100;
			}
			if (vy > 0)
			{
				vy += dt * -100 * 4;
			}
			else
			{
				if (vy < 0)
				{
					vy += dt * 100 * 4;
				}
				else
					vy = 0;
			}
			flipX = false;
		}
		else
		{
			if (this->GetCollisionBox().right <= target->GetCollisionBox().left)
			{
				if (vx < 100)
				{
					vx += dt * 100 * 4;
				}
				else
				{
					vx = 100;
				}
				if (vy > 0)
				{
					vy += dt * -100 * 4;
				}
				else
				{
					if (vy < 0)
					{
						vy += dt * 100 * 4;
					}
					else
						vy = 0;
				}
				flipX = true;
			}
			else
			{
				state = SOPHIA_BULLET_H_UP;
				if (this->GetCollisionBox().top <= target->GetCollisionBox().bottom)
				{
					if (vx > 0)
					{
						vx += dt * -100 * 4;
					}
					else
					{
						if (vx < 0)
						{
							vx += dt * 100 * 4;
						}
						else
							vx = 0;
					}
					if (vy < 100)
					{
						vy += dt * 100 * 4;
					}
					else
					{
						vy = 100;
					}
					up = false;
					down = true;
				}
				else
				{
					if (this->GetCollisionBox().bottom >= target->GetCollisionBox().top)
					{
						if (vx > 0)
						{
							vx += dt * -100 * 4;
						}
						else
						{
							if (vx < 0)
							{
								vx += dt * 100 * 4;
							}
							else
								vx = 0;
						}
						if (vy > -100)
						{
							vy += dt * -100 * 4;
						}
						else
						{
							vy = -100;
						}
						up = true;
						down = false;
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