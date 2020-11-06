
#include "DInput.h"
#include "Animator_Floater.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Floater.h"

Floater::Floater()
{
	animator = new Animator_Floater();
	animator->AddAnimation(floaterFly);
	animator->AddAnimation(floaterIdle);
	state = floaterIdle;
	vx = 150;
	vy = 150;
}
void Floater::OnCollisionEnter(CollisionEvent e)
{
	if (e.ny < 0   && dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		vy = -150;
	}
	if (e.ny > 0 && dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		vy = 150;
	}
	if (e.nx < 0  && dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		vx = -150;
	}
	if (e.nx > 0 && dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		vx = 150;
	}
}
void Floater::Update(float dt)
{
	
	DebugOut(L"Vx %f \n",vx);
	DebugOut(L"Vy %f \n", vy);
	
}

void Floater::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Floater::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 5;
	colRect.right = x + 5;
	colRect.top = y - 4;
	colRect.bottom = y + 4;

	return colRect;
}
