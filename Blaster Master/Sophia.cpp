#include "Sophia.h"
#include "Animator_Sophia.h"
#include "Debug.h"

#define STATE_SOPHIA_IDLE 29801
#define STATE_SOPHIA_MOVE 29805
#define STATE_SOPHIA_JUMP 29811
#define STATE_SOPHIA_FALL 29815
#define STATE_SOPHIA_MOVE_45 29821
#define STATE_SOPHIA_MOVE_90 29825
#define STATE_SOPHIA_JUMP_90 29831
#define STATE_SOPHIA_FALL_90 29835
#define STATE_SOPHIA_MOVE_TURN 29841
#define STATE_SOPHIA_MOVE_TURN 29845

#define STATE_SOPHIA_IDLE_90 29851

Sophia::Sophia()
{
	//boolean
	flipX = false;
	moving = false;
	gun_up = 0;
	//wheel
	last = 0;
	//gun
	last_gun = 0;
	//state
	state = STATE_SOPHIA_IDLE;
	//animator
	animator = new Animator_Sophia();
	//Idle, gun 0
	animator->AddAnimation(29801);
	animator->AddAnimation(29802);
	animator->AddAnimation(29803);
	animator->AddAnimation(29804);
	//Moving, gun 0
	animator->AddAnimation(29805);
	animator->AddAnimation(29806);
	animator->AddAnimation(29807);
	animator->AddAnimation(29808);
	//Jumping, gun 0
	animator->AddAnimation(29811);
	animator->AddAnimation(29812);
	animator->AddAnimation(29813);
	animator->AddAnimation(29814);
	//Falling, gun 0
	animator->AddAnimation(29815);
	animator->AddAnimation(29816);
	animator->AddAnimation(29817);
	animator->AddAnimation(29818);
	//Moving, gun 45
	animator->AddAnimation(29821);
	animator->AddAnimation(29822);
	animator->AddAnimation(29823);
	animator->AddAnimation(29824);
	//Moving, gun 90
	animator->AddAnimation(29825);
	animator->AddAnimation(29826);
	animator->AddAnimation(29827);
	animator->AddAnimation(29828);
	//Jumping, gun 90
	animator->AddAnimation(29831);
	animator->AddAnimation(29832);
	animator->AddAnimation(29833);
	animator->AddAnimation(29834);
	//Falling, gun 90
	animator->AddAnimation(29835);
	animator->AddAnimation(29836);
	animator->AddAnimation(29837);
	animator->AddAnimation(29838);
	//Moving, gun turn
	animator->AddAnimation(29841);
	animator->AddAnimation(29842);
	animator->AddAnimation(29843);
	animator->AddAnimation(29844);
	//jumping, gun turn
	animator->AddAnimation(29845);
	animator->AddAnimation(29846);
	animator->AddAnimation(29847);
	animator->AddAnimation(29848);
	//idle, gun 90
	animator->AddAnimation(29851);
	animator->AddAnimation(29852);
	animator->AddAnimation(29853);
	animator->AddAnimation(29854);
}

FRECT Sophia::GetCollisionBox()
{
	FRECT colRect;
	switch (gun_up)
	{
	case 0: colRect.left = this->x - 24 / 2;
			colRect.right = this->x + 24 / 2;
			colRect.top = this->y - 14 / 2;
			colRect.bottom = this->y + 22 / 2;  break;
	case 45:
			colRect.left = this->x - 24 / 2;
			colRect.right = this->x + 24 / 2;
			colRect.top = this->y - 6 / 2;
			colRect.bottom = this->y + 30 / 2; break;
	case 90:
			colRect.left = this->x - 24 / 2;
			colRect.right = this->x + 24 / 2;
			colRect.top = this->y + 2 / 2;
			colRect.bottom = this->y + 38 / 2; break;
	}
	return colRect;
}

void Sophia::OnCollisionEnter(CollisionEvent e)
{
	if (e.ny < 0)
	{
		//vy = 0;
		onTheGround = true;
	}
}

void Sophia::Update(float dt)
{
	vy += 300 * dt;

	if (DInput::KeyPress(DIK_LEFT))
	{
		vx = -100;
		flipX = true;
		moving = true;
	}
	else if (DInput::KeyPress(DIK_RIGHT))
	{
		vx = 100;
		flipX = false;
		moving = true;
	}
	else
	{
		vx = 0;
		moving = false;
	}

	if (DInput::KeyPress(DIK_SPACE) && canJump)
	{
		vy = -150;
		onTheGround = false;
		canJump = false;
	}

	if (onTheGround && !DInput::KeyPress(DIK_SPACE))
	{
		canJump = true;
	}
	DWORD now = GetTickCount();
	if (DInput::KeyPress(DIK_UP))
	{
		if (gun_up == 0)
		{
			/*if (flipX)
			{
				x += 2;
			}
			else
			{
				x -= 2;
			}*/
			y -= 4;
			gun_up = 45;
			last_gun = now;
		}
		else
		{
			if (gun_up == 45 && now - last_gun > 200)
			{
				/*if (flipX)
				{
					x += 2;
				}
				else
				{
					x -= 2;
				}*/
				y -= 4;
				gun_up = 90;
			}
		}
	}
	else
	{
		if (gun_up == 90)
		{
			gun_up = 45;
			/*if (flipX)
			{
				x -= 2;
			}
			else
			{
				x += 2;
			}*/
			y += 4;
		}
		else
		{
			if (gun_up == 45)
			{
				/*if (flipX)
				{
					x -= 2;
				}
				else
				{
					x += 2;
				}*/
				y += 2;
				gun_up = 0;
			}
		}
	}
	StateChange();
}

void Sophia::StateChange()
{
	dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = false;
	if (vy < 0)
	{
		if (state != STATE_SOPHIA_JUMP)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		}
		state = STATE_SOPHIA_JUMP;
		return;
	}
	if (vy > 0 && !onTheGround)
	{
		if (state != STATE_SOPHIA_FALL)
		{
			y -= 5;
		}
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		state = STATE_SOPHIA_FALL;
		return;
	}
	if (moving && gun_up == 45)
	{
		if (state != STATE_SOPHIA_MOVE_45)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
			state = STATE_SOPHIA_MOVE_45;
		}
		return;
	}
	if (moving && gun_up == 90)
	{
		if (state != STATE_SOPHIA_MOVE_90)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
			state = STATE_SOPHIA_MOVE_90;
		}
		return;
	}
	if(moving && gun_up == 0)
	{
		if (state != STATE_SOPHIA_MOVE)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
			state = STATE_SOPHIA_MOVE;
		}
		return;
	}
	if (vx == 0 && gun_up == 45)
	{
		if (state != STATE_SOPHIA_MOVE_45)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
			state = STATE_SOPHIA_MOVE_45;
		}
		return;
	}
	if (vx == 0 && gun_up == 90)
	{
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		state = STATE_SOPHIA_IDLE_90;
		return;
	}
	if (vx == 0 && gun_up == 0)
	{
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		state = STATE_SOPHIA_IDLE;
		return;
	}
}

void Sophia::Render()
{
	if (moving)
	{
		DWORD now = GetTickCount();
		if (dynamic_cast<Animator_Sophia*>(animator)->wheel == -1)
		{
			dynamic_cast<Animator_Sophia*>(animator)->wheel = 1;
			last = now;
		}
		else
		{
			DWORD t = 50;
			if (now - last > t)
			{
				dynamic_cast<Animator_Sophia*>(animator)->wheel++;
				last = now;
				if (dynamic_cast<Animator_Sophia*>(animator)->wheel == 4) dynamic_cast<Animator_Sophia*>(animator)->wheel = 1;
			}

		}
	}
	DebugOut(L"%d\n",state);
	animator->Draw(state + dynamic_cast<Animator_Sophia*>(animator)->wheel -1, x, y, flipX);
}