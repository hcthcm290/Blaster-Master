#include "VisionBox.h"

VisionBox::VisionBox(int left, int right, int top, int bottom)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}
FRECT VisionBox::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = left;
	colRect.right = right;
	colRect.top = top;
	colRect.bottom = bottom;
	return colRect;
}
void VisionBox::OnCollisionEnter(CollisionEvent e)
{

}

void VisionBox::Update(float dt)
{
}

void VisionBox::Render()
{

}
