#include "Orb.h"

D3DRECT Orb::GetCollisionBox()
{
	D3DRECT colRect;
	colRect.x1 = this->x;
	colRect.x2 = this->x + ORB_COLLISION_BOX_WIDTH;
	colRect.y1 = this->y;
	colRect.y2 = this->y + ORB_COLLISION_BOX_HEIGHT;
	return colRect;
}

void Orb::Update(DWORD dt)
{

}

void Orb::Render()
{
	animator.Draw(1001, x, y, false);
}
