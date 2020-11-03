#include "Orb.h"

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
	if (e.ny == -1)
	{
		vy = 0;
		onTheGround = true;
	}
}

void Orb::Update(float dt)
{
	vy += 10;

	if (DInput::KeyDown(DIK_LEFT))
		vx = -50;
	else if (DInput::KeyDown(DIK_RIGHT))
		vx = 50;
	else
		vx = 0;

	if (DInput::KeyDown(DIK_SPACE) && onTheGround)
	{
		vy = -300;
		onTheGround = false;
	}
}

void Orb::Render()
{
	animator->Draw(20902, x, y, false);
}


