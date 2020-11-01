#include "Jumper.h"
#include "DInput.h"
#include "Animator_Jumper.h"

Jumper::Jumper()
{
	animator = new Animator_Jumper();
	state = jumperWalk;
	currentY = 50;
}
void Jumper::Update(float dt)
{
	if (DInput::KeyDown(DIK_X) && keyRelease)
	{
		
		if (this->state == jumperWalk)
			this->state = jumperIdle;
		else this->state = jumperWalk;
		//toggle
		keyRelease = false;
	}

	if (!DInput::KeyDown(DIK_X))
	{
		keyRelease = true;
	}
	//Changing walking status
	if (this->state == jumperWalk || isJumpDown||isJumpUp)
	{
		Vx = direction * jumperSpeed;
		if (DInput::KeyDown(DIK_SPACE) && keyRelease)
		{
			isJumpUp = true;
			isJumpDown = false;
			keyRelease = false;
		}
		if (!DInput::KeyDown(DIK_SPACE))
		{
			keyRelease = true;
		}
		if (isJumpUp)
		{
			Vx = Vx * cos(70);
			Vy = sin(70) - accelerate;
			if (y <= maxJumpTop)
			{
				isJumpDown = true;
				isJumpUp = false;
			}
		}
		if (isJumpDown)
		{
			Vx = Vx * cos(70);
			Vy = (0.5) * accelerate;
			if (y >= currentY)
			{
				isJumpDown = false;
				Vy = 0;
			}
		}
		x += Vx*float(dt)/1000; //change Horizonal Location
		y += Vy*float(dt) / 1000;
		if (x <= 0)
		{
			flip = true;
			direction = 1;
		}
		if (x >= 150)
		{
			flip = false;
			direction = -1;
		}
	}
}

void Jumper::Render()
{
	animator->Draw(state, x, y, flip);
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
