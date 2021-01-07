#include "Orb.h"
#include "ColliableBrick.h"
#include "Mine.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"
#include "PlayScene.h"

Orb::Orb()
{
	//set HP
	HP = 30;

	animator = new Animator();
	animator->AddAnimation(orbUOD);
	animator->AddAnimation(orbFly);
	state = orbFly;
	flip = false;
	vx = vy = 0;
}

FRECT Orb::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - ORB_COLLISION_BOX_WIDTH / 2;
	colRect.right = this->x + ORB_COLLISION_BOX_WIDTH / 2;
	colRect.top = this->y - ORB_COLLISION_BOX_HEIGHT / 2;
	colRect.bottom = this->y + ORB_COLLISION_BOX_HEIGHT / 2;
	return colRect;
}

void Orb::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL)
	{
		if (e.ny < 0)
		{
			ver_direction = -1;
			hitWall = true;
		}
		if (e.ny>0)
		{
			ver_direction = 1;
			hitWall = true;
		}
		if (e.nx < 0)
		{
			flip = false;
			hor_direction = -1;
		}
		if (e.nx>0)
		{
			flip = true;
			hor_direction = 1;
		}
	}
}

void Orb::Update(float dt)
{
	vx = hor_direction * orbSpeed;
	if (vy != 0)
		vx = 0;
	waitForChange += dt;
	if (this->state == orbUOD)
	{
		turnAround += dt;
		if (turnAround > 0.3)
		{
			this->state = orbFly;
			turnAround = 0;
			vy = 0;
		}
	}
	if (waitForChange >= 5)
	{
		this->state = orbUOD;
		waitForChange = 0;
		if (hitWall)
		{
			hitWall = false;
		}
		else
		{
			float random = RandomFloat(-1, 1);
			if (random < 0) ver_direction = -1;
			else ver_direction = 1;
		}
		vy = ver_direction * orbSpeed;
		vx = 0;
	}
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(5);
	}
}

void Orb::Render()
{
	if (inv != -1) {
		animator->Draw(state, x, y, flip, 0, Color[inv]);
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
		animator->Draw(state, x, y, flip);
	}
}


