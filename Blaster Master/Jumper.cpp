#include "Jumper.h"
#include "DInput.h"
#include "Debug.h"
#include <string> 

void Jumper::Update(float dt)
{
	float speed = 100;

	if (DInput::KeyDown(DIK_LEFT) && !DInput::KeyDown(DIK_RIGHT))
		vx = -speed;
	else if (DInput::KeyDown(DIK_RIGHT) && !DInput::KeyDown(DIK_LEFT))
		vx = speed;
	else
		vx = 0;

	if (DInput::KeyDown(DIK_UP))
		vy = -speed;
	else if (DInput::KeyDown(DIK_DOWN))
		vy = speed;
	else
		vy = 0;
}

void Jumper::Render()
{
	animator.Draw(2001, x, y, false);
}

FRECT Jumper::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 8;
	colRect.right = x + 8;
	colRect.top = y - 13;
	colRect.bottom = y + 13;

	return colRect;
}
