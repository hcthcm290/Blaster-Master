#include "Orb.h"
#include "ColliableBrick.h"
#include "Mine.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"

Orb::Orb()
{
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
}

void Orb::Render()
{
	animator->Draw(state, x, y, flip);
}


