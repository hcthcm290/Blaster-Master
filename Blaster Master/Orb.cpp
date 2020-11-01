#include "Orb.h"

FRECT Orb::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - ORB_COLLISION_BOX_WIDTH / 2;
	colRect.right = this->x + ORB_COLLISION_BOX_WIDTH / 2;
	colRect.top = this->y - ORB_COLLISION_BOX_HEIGHT / 2;
	colRect.bottom = this->y + ORB_COLLISION_BOX_HEIGHT / 2;
	return colRect;
}

void Orb::Update(float dt)
{
}

void Orb::Render()
{
	animator->Draw(20902, x, y, false);
}
