#include "BigJason.h"
#include "Animator.h"

BigJason::BigJason()
{
	animator = new Animator();
	animator->AddAnimation(29901);
}

void BigJason::Update(float dt)
{
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
