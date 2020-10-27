#include "Jumper.h"
#include "DInput.h"
#include "Animator_Jumper.h"

#define jumperWalk Constant::_JUMPER_WALK_
#define	jumperIdle Constant::_JUMPER_IDLE_
Jumper::Jumper()
{
	animator = new Animator_Jumper();
	state = jumperWalk;
}
void Jumper::Update(DWORD dt)
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
	if (this->state == jumperWalk)
	{
		x +=  direction * 20 * float(dt)/1000;
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
