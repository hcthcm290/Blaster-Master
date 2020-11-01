#include "Dome.h"

void Dome::Update(float dt)
{
	if (DInput::KeyDown(DIK_LEFT))
	{
		vx = -35;
	}
	else if (DInput::KeyDown(DIK_RIGHT))
	{
		vx = 35;
	}
	else
	{
		vx = 0;
	}

	x += vx * dt/(float)1000;
}

void Dome::Render()
{
	animator->Draw(1003, x, y, false);
}
