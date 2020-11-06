#include "Jumper.h"
#include "DInput.h"
#include "Animator_Jumper.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Orb.h"

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
			mini_waitForJump = 0;
		}
		onTheGround = true;
	}
	if (dynamic_cast<Orb*>(e.pGameObject))
	{

	}
}
void Jumper::Update(float dt)
{
	//if (!canJump)
	//{
	//	                         
	//	if (onTheGround)
	//	{
	//		DebugOut(L"OnTheGround\n");
	//	}
	//	else
	//	{
	//		DebugOut(L"not OnTheGround\n");
	//	}
	//}

	///*DebugOut(L"%f\n", waitForJump);
	//DebugOut(L"count %d\n", jumpCount);
	
	/*if (!onTheGround)
	{
		jumpTime -= dt;

		if (jumpTime <= 0 && vy < 0)
		{
			vy = 0;
			accelerateY = 700;
		}
	}*/

	float Character_X = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	float Character_Y = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y;
	float distance = x - Character_X;
	if((onTheGround && waitForJump>=2)|| mini_waitForJump>=0.1 && onTheGround && (jumpCount>1))	direction = distance/ abs(distance);

	if (abs(distance) <= 100)
	{
		if (abs(distance) <= 30)
			trigger = true;
		else trigger = false;
		this->state = jumperWalk;
	}
	else trigger = false;

	if (!onTheGround)	this->state = jumperIdle;

	if (waitForJump >= 2 && onTheGround)
	{
		jumpCount = 4;
		waitForJump = -99;
		canJump = true;
	}
	if (mini_waitForJump >= 0.1 && jumpCount>0 && onTheGround)
	{
		jumpCount--;
		canJump = true;
		mini_waitForJump = -99;
	}
	if (jumpCount == 0)
	{
		canJump = false;
	}
	//Changing walking status
	if (this->state == jumperWalk)
	{
		waitForJump += dt;
		mini_waitForJump += dt;
		vx = -30 * direction;
		if (canJump && trigger)
		{
			vy = -150;
			jumpTime = 0.3;
			onTheGround = false;
			canJump = false;
		}
	}
	if (direction > 0 )
		flip = false;
	else
		flip = true;

	vy += 300 * dt;
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
