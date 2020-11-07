#include "Orb.h"
#include "ColliableBrick.h"
#include "Mine.h"

Orb::Orb()
{
	animator->AddAnimation(20902);
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
	if (e.ny < 0 && dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL)
	{
		//vy = 0;
		onTheGround = true;
	}

	if (dynamic_cast<Mine*>(e.pGameObject) != NULL)
	{
		auto csa = 57489;
	}

	if (e.nx != 0)
	{
		auto csa = 432;
	}
}

void Orb::Update(float dt)
{
	vy += 300*dt;

	if (DInput::KeyPress(DIK_LEFT))
		vx = -50;
	else if (DInput::KeyPress(DIK_RIGHT))
		vx = 50;
	else
		vx = 0;

	if (DInput::KeyPress(DIK_SPACE) && canJump)
	{
		vy = -300;
		onTheGround = false;
		canJump = false;
	}

	if (onTheGround && !DInput::KeyPress(DIK_SPACE))
	{
		canJump = true;
	}
}

void Orb::Render()
{
	animator->Draw(20902, x, y, false);
}


