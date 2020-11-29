#include "Teleporter.h"

Teleporter::Teleporter()
{
	//set HP
	HP = 40;
	//animator
	animator->AddAnimation(21001);
}
FRECT Teleporter::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 26 / 2;
	colRect.right = this->x + 26 / 2;
	colRect.top = this->y - 33 / 2;
	colRect.bottom = this->y + 33 / 2;
	return colRect;
}

void Teleporter::OnCollisionEnter(CollisionEvent e)
{
	
}
void Teleporter::Update(float dt)
{

}

void Teleporter::Render()
{
	//animator.Draw(20902, x, y, false);
}