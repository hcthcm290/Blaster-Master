#include "Sophia.h"
#include "Animator_Sophia.h"
#include "Debug.h"
#include "Sophia_Bullet_1.h"
#include "Sophia_Bullet_Homing.h"
#include "ColliableBrick.h"
#include "PlayScene.h"
#include "Enemy.h"
#include "VisionBox.h"
#include "Camera.h"

#define STATE_SOPHIA_IDLE 29801
#define STATE_SOPHIA_MOVE 29805
#define STATE_SOPHIA_JUMP 29811
#define STATE_SOPHIA_FALL 29815
#define STATE_SOPHIA_MOVE_45 29821
#define STATE_SOPHIA_MOVE_90 29825
#define STATE_SOPHIA_JUMP_90 29831
#define STATE_SOPHIA_FALL_90 29835
#define STATE_SOPHIA_MOVE_TURN 29841
#define STATE_SOPHIA_JUMP_TURN 29845

#define STATE_SOPHIA_IDLE_90 29851
#define STATE_SOPHIA_JUMP_45 29855
#define STATE_SOPHIA_FALL_45 29861
#define STATE_SOPHIA_FALL_TURN 29865

#define STATE_SOPHIA_SHIFT 29871
#define STATE_SOPHIA_SLEEP 29875

Sophia::Sophia()
{
	//sx, sy
	sx = x;
	sy = y;
	//boolean
	flipX = false;
	moving = false;
	gun_up = 0;
	gun_turn = false;
	//wheel
	last = 0;
	//gun
	last_gun = 0;
	start_gunturn = 0;
	//flipX
	last_flipX = false;
	//bullet
	last_bullet = GetTickCount();
	//state
	state = STATE_SOPHIA_IDLE;
	//animator
	animator = new Animator_Sophia();
}

FRECT Sophia::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 24 / 2;
	colRect.right = this->x + 22 / 2;
	colRect.top = this->y - 20 / 2;
	colRect.bottom = this->y + 22 / 2;
	return colRect;
}

void Sophia::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != nullptr)
	{
		if (e.ny < 0)
		{
			//vy = 0;
			onTheGround = true;
		}
	}
}

void Sophia::Update(float dt)
{
	//if Sophia takes player control, switch it to IDLE (made by TrV)
	if (state == STATE_SOPHIA_SLEEP && dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer() == this) {
		state = STATE_SOPHIA_IDLE;
	}

	DWORD now = GetTickCount();
	if (state == STATE_SOPHIA_SHIFT || state == STATE_SOPHIA_SLEEP)
	{
		vx = 0;
		vy = 0;
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		if (now - start_shift > 300)
		{
			state = STATE_SOPHIA_SLEEP;
		}
	}
	else
	{
		//reset position
		if (DInput::GetInstance()->KeyPress(DIK_R)) {
			x = 1120;
			y = 1136;
			state = STATE_SOPHIA_IDLE;
			return;
		}

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

		if (DInput::KeyPress(DIK_X) && canJump)
		{
			vy = -150;
			onTheGround = false;
			canJump = false;
		}

		if (onTheGround && !DInput::KeyPress(DIK_X))
		{
			canJump = true;
		}
		if (DInput::KeyPress(DIK_UP))
		{
			if (gun_up == 0)
			{
				gun_up = 45;
				last_gun = now;
			}
			else
			{
				if (gun_up == 45 && now - last_gun > 200)
				{
					gun_up = 90;
				}
			}
		}
		else
		{
			if (gun_up == 90)
			{
				gun_up = 45;
				last_gun = now;
			}
			else
			{
				if (gun_up == 45 && now - last_gun > 200)
				{
					gun_up = 0;
				}
			}
		}
		if (last_flipX != flipX)
		{
			gun_turn = true;
			start_gunturn = now;
		}
		else
		{
			if (now - start_gunturn > 200)
			{
				gun_turn = false;
			}
		}
		if (DInput::KeyPress(DIK_Z))
		{
			if (DInput::KeyPress(DIK_DOWN))
			{
				if(now - last_bullet > 300)
					ShootHoming();
			}
			else
			{
				if (now - last_bullet > 300 && (new Sophia_Bullet_1)->count < 3)
				{
					last_bullet = now;
					bool up = false;
					if (gun_up == 90)
					{
						up = true;
					}
					auto bullet = new Sophia_Bullet_1(up, !flipX);
					bullet->SetPosition(x, y);
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
				}
			}
		}
		StateChange();
		last_flipX = flipX;
		if (onTheGround && DInput::KeyDown(DIK_LSHIFT) && (GetTickCount64() - switchDelay >= 1000))
		{
			jason = new Jason(JasonCurrentHealth, x, y - 10, this);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetPlayer(jason);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(jason);
			state = STATE_SOPHIA_SHIFT;
			start_shift = now;
		}
	}
}

void Sophia::ShootHoming()
{
	FRECT camera = Camera::GetInstance()->GetCollisionBox();
	CGameObject* vision = new VisionBox(camera.left, camera.right, camera.top, camera.bottom);
	int count = 0;

	for (auto enemy : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
	{
		if (dynamic_cast<Enemy*>(enemy) != nullptr)
		{
			if (count <= 4)
			{
				last_bullet = GetTickCount();
				bool up = false;
				if (gun_up == 90)
				{
					up = true;
				}
				if (CollisionSystem::CheckOverlap(enemy, vision))
				{
					auto bullet = new Sophia_Bullet_Homing(!flipX, dynamic_cast<DynamicObject*>(enemy));
					bullet->SetPosition(x, y);
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
				}
			}
			else
			{
				break;
			}
		}
	}
	delete vision;
}

void Sophia::StateChange()
{

	dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = false;
	if (vy < 0)
	{
		if (state != STATE_SOPHIA_JUMP && state != STATE_SOPHIA_JUMP_90 && state != STATE_SOPHIA_JUMP_45)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		}
		if (gun_up == 90)
		{
			state = STATE_SOPHIA_JUMP_90;
		}
		else
		{
			if (gun_up == 45)
			{
				state = STATE_SOPHIA_JUMP_45;
			}
			else
			{
				if (gun_turn)
					state = STATE_SOPHIA_JUMP_TURN;
				else
					state = STATE_SOPHIA_JUMP;
			}
		}
		return;
	}
	if (vy > 0 && !onTheGround)
	{
		if (state != STATE_SOPHIA_FALL && state != STATE_SOPHIA_FALL_90 && state != STATE_SOPHIA_FALL_45 && state != STATE_SOPHIA_FALL_TURN)
		{
			y -= 5;
		}
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		if (gun_up == 90)
		{
			state = STATE_SOPHIA_FALL_90;
		}
		else
		{
			if (gun_up == 45)
			{
				state = STATE_SOPHIA_FALL_45;
			}
			else
			{
				if (gun_turn)
					state = STATE_SOPHIA_FALL_TURN;
				else
					state = STATE_SOPHIA_FALL;
			}
		}
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
			if (gun_turn)
				state = STATE_SOPHIA_MOVE_TURN;
			else
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
		if (gun_turn)
			state = STATE_SOPHIA_MOVE_TURN;
		else
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
	if (gun_up == 0)
	{
		sx = x;
		sy = y;
	}
	else
	{
		if (gun_up == 45)
		{
			if (flipX)
			{
				sx = x + 1;
			}
			else
			{
				sx = x - 1;
			}
			sy = y - 4;
		}
		else
		{
			if (flipX)
			{
				sx = x + 3;
			}
			else
			{
				sx = x - 3;
			}
			sy = y - 8;
		}
	}
	//DebugOut(L"%d\n", state);
	if (state == STATE_SOPHIA_SHIFT || state == STATE_SOPHIA_SLEEP)
	{
		animator->Draw(state, sx, sy, flipX);	
	}
	else
	{
		animator->Draw(state + dynamic_cast<Animator_Sophia*>(animator)->wheel - 1, sx, sy, flipX);
	}
}

void Sophia::Awake(int JasonHealth) {
	state = STATE_SOPHIA_SHIFT;
	switchDelay = GetTickCount64();
	JasonCurrentHealth = JasonHealth;
}