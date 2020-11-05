#include "Jumper.h"
#include "DInput.h"
#include "Animator_Jumper.h"
#include "ColliableBrick.h"
#include "Debug.h"

Jumper::Jumper()
{
	animator = new Animator_Jumper();
	animator->AddAnimation(jumperWalk);
	animator->AddAnimation(jumperIdle);
	state = jumperIdle;
}
void Jumper::OnCollisionEnter(CollisionEvent e)
{
	if (e.ny < 0 && dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		if (!onTheGround)
		{
			waitForJump = 0;
		}
		onTheGround = true;
		
	}
}
void Jumper::Update(float dt)
{
	DebugOut(L"%f\n", waitForJump);
	vy += 300*dt;
	float Character_X = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	float Character_Y = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y;
	float distance = x - Character_X;
	if(onTheGround && waitForJump>=2)	direction = distance/ abs(distance);

	if (abs(distance) <= 100)
	{
		if (abs(distance) <= 50)
			trigger = true;
		else trigger = false;
		
	this->state = jumperWalk;
	}
	if (waitForJump >= 2)
	{
		canJump = true;
		waitForJump = -99;
	}
	//Changing walking status
	if (this->state == jumperWalk)
	{
		waitForJump += dt;
		vx = -30 * direction;
		if (trigger && canJump)
		{
			vy = -150;
			onTheGround = false;
			canJump = false;
		}
		/*if (onTheGround && trigger)
		{
			canJump = true;
		}*/
	}
	if (direction > 0 )
		flip = false;
	else
		flip = true;
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
