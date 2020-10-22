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

D3DRECT Jumper::GetCollisionBox()
{
	D3DRECT colRect;
	colRect.x1 = x - 8;
	colRect.x2 = x + 8;
	colRect.y1 = y - 13;
	colRect.y2 = y + 13;

	return colRect;
}
