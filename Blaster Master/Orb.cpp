#include "Orb.h"

D3DRECT Orb::GetCollisionBox()
{
	D3DRECT colRect;
	colRect.x1 = this->x - ORB_COLLISION_BOX_WIDTH / 2;
	colRect.x2 = this->x + ORB_COLLISION_BOX_WIDTH / 2;
	colRect.y1 = this->y - ORB_COLLISION_BOX_HEIGHT / 2;
	colRect.y2 = this->y + ORB_COLLISION_BOX_HEIGHT / 2;
	return colRect;
}

void Orb::Update(float dt)
{
}

void Orb::Render()
{
	animator.Draw(1001, x, y, false);
}
