#include "Jumper.h"
#include "DInput.h"
#include "Animator_Jumper.h"

Jumper::Jumper()
{
	animator = new Animator_Jumper();
	animator->AddAnimation(jumperWalk);
	animator->AddAnimation(jumperIdle);
	state = jumperIdle;
}
void Jumper::OnCollisionEnter(CollisionEvent e)
{
	if (e.ny < 0)
	{
		onTheGround = true;
	}
}
void Jumper::Update(float dt)
{
	vy += 300*dt;
	float Character_X=dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	float Character_Y = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y;
	float distance = x - Character_X;
	int direction = distance/ abs(distance);
	if (abs(distance) <= 200)
	{
		
			trigger = true;
		
		this->state = jumperWalk;
	}
	else
	{
		trigger = false;
		this->state = jumperIdle;
		vy = 0;
		vx = 0;
	}
	//Changing walking status
	if (this->state == jumperWalk)
	{
		vx = -1 * direction * 20;
		if (trigger && canJump)
		{
			vy = -100;
			onTheGround = false;
			canJump = false;
		}
		if (onTheGround && trigger)
		{
			canJump = true;
		}
	}
	if (direction > 0)
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
