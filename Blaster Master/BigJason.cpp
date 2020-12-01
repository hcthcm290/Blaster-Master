#include "BigJason.h"
#include "Animator.h"
#include "PInput.h"

BigJason::BigJason()
{
	animator = new Animator();
	animator->AddAnimation(29901);
}

void BigJason::Update(float dt)
{
	//check 
	bulletManager->CheckBullet();
	bulletManager->CheckCheat();

	if (DInput::KeyPress(DIK_LEFT))
	{
		vx = -40;
	}
	else if (DInput::KeyPress(DIK_RIGHT))
	{
		vx = 40;
	}
	else
	{
		vx = 0;
	}

	if (DInput::KeyPress(DIK_UP))
	{
		vy = -40;
	}
	else if (DInput::KeyPress(DIK_DOWN))
	{
		vy = 40;
	}
	else
	{
		vy = 0;
	}

	if (PInput::KeyDown(SHOOT)) {
		int dx = 0;
		int dy = 0;
		if (vx != 0) dx = (vx < 0 ? -1 : 1);
		else if (vy != 0) dy = (vy < 0 ? -1 : 1);
		bulletManager->Fire(x, y, dx, dy);
	}
}

void BigJason::Render()
{
	animator->Draw(29901, x, y, false);
}

FRECT BigJason::GetCollisionBox()
{
	int width = 20;
	int height = 6;

	// center of collision box is at the shadow of jason
	int deltaShiftY = 12;

	return FRECT(x - width / 2, y + deltaShiftY - height / 2, x + width / 2, y + deltaShiftY + height / 2);
}

bool BigJason::IsInvulnerable()
{
	return false;
}
