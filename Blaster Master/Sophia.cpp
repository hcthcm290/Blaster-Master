#include "Sophia.h"
#include "Animator_Sophia.h"
#include "Debug.h"
#include "Sophia_Bullet_1.h"
#include "Sophia_Bullet_Homing.h"
#include "Sophia_Bullet_Rocket.h"
#include "ColliableBrick.h"
#include "PlayScene.h"
#include "Enemy.h"
#include "VisionBox.h"
#include "Camera.h"
#include "PInput.h"
#include "TheEye.h"
#include "SoundManager.h"
#include "Sophia_Bullet_Thunder.h"
#include "IDSceneConstant.h"

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
#define STATE_SOPHIA_SHIFT_IN 29872

#define STATE_SOPHIA_DIE 29881

Sophia::Sophia()
{
	vx = 0;
	currentColor = 0;
	//Setup HP
	HP = 100;
	maxHP = HP;
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
	//time
	lastDamageTime = GetTickCount();
	//state
	state = STATE_SOPHIA_IDLE;
	//animator
	animator = new Animator_Sophia();
	onTheGround = true;
	start_shift = GetTickCount();

	TheEye::GetInstance()->SetSophia(this);
}

float Sophia::GetEnterGateSpeed()
{
	return 50;
}

Sophia::~Sophia()
{
	
}
FRECT Sophia::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 24 / 2;
	colRect.right = this->x + 22 / 2;
	if (onTheGround)
	{
		colRect.top = this->y - 5.5;
		colRect.bottom = this->y + 10;
	}
	else if (!onTheGround && vy < 0)
	{
		colRect.top = this->y - 9;
		colRect.bottom = this->y + 6.5;
	}
	else
	{
		colRect.top = this->y - 5.5;
		colRect.bottom = this->y + 10;
	}
	return colRect;
}

void Sophia::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != nullptr)
	{
		if (e.ny < 0)
		{
			//vy = 0;
			if (!onTheGround)
			onTheGround = true;
		}
	}
}

void Sophia::Update(float dt)
{
	if (animator == NULL)
	{
		animator = new Animator_Sophia();
	}
	if (DInput::KeyDown(DIK_P))
	{
		CGame::GetInstance()->SwitchScene(2);
		return;
	}

	if (HP == 0)
	{
		vx = 0;
		vy = 0;
		if (state != STATE_SOPHIA_DIE && state != 20000)
		{
			state = STATE_SOPHIA_DIE;
			SoundManager::GetInstance()->PlaySoundW("SophiaDie.wav");
			die = GetTickCount();
		}
		else
		{
			if (GetTickCount() - die > 500)
			{
				state = 20000;
				invincible = 0;
				return;
			}
		}
		return;
	}
	if (invincible >= 0)
	{
		invincible -= dt * 1000;
	}
	else
	{
		invincible = 0;
	}

	DWORD now = GetTickCount();
	if (isInvincible() && currentColor == 0)
	{
		//currentColor = 2;
		currentColor = 4;
		lastDamageTime = now;
	}
	if (isInvincible())
	{
		if (now - lastDamageTime > 100)
		{
			switch (currentColor)
			{
			case 4: currentColor = 3; break;
			case 3: currentColor = 2; break;
			case 2: currentColor = 1; break;
			case 1: currentColor = 4; break;
			}
			//currentColor = 5 - currentColor;
			lastDamageTime = now;
		}
	}
	else
	{
		currentColor = 0;
	}
	if (state == STATE_SOPHIA_SHIFT || state == STATE_SOPHIA_SLEEP || state == STATE_SOPHIA_SHIFT_IN)
	{
		switching = true;
		vx = 0;
		vy = 0;
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		if (now - start_shift > 300)
		{
			if(state == STATE_SOPHIA_SHIFT)
				state = STATE_SOPHIA_SLEEP;
			else
			{
				if (state == STATE_SOPHIA_SHIFT_IN)
				{
					state = STATE_SOPHIA_IDLE;
					onTheGround = true;
				}
			}
		}
	}
	else
	{
		switching = false;
		if (now - start_shift < 1000)
			switching = true;
		//reset position
		/*if (PInput::KeyPressed()) {
			x = 1120;
			y = 1136;
			state = STATE_SOPHIA_IDLE;
			return;
		}*/

		vy += 480 * dt;
		int speed = 170;

		if (PInput::KeyPressed(LEFT))
		{
			//vx = -100;
			if (vx <= -100)
			{
				vx = -100;
			}
			else
			{
				if (vx == 0)
					vx = -25;
				vx -= speed * 3 * dt;
			}
			flipX = true;
			moving = true;
		}
		else if (PInput::KeyPressed(RIGHT))
		{
			//vx = 100;
			if (vx >= 100)
			{
				vx = 100;
			}
			else
			{
				if (vx == 0)
					vx = 25;
				vx += speed * 3* dt;
			}
			flipX = false;
			moving = true;
		}
		else
		{
			//vx = 0;
			if (vx > 0)
			{
				vx -= speed * 3 * dt;
				if (vx < 0)
					vx = 0;
			}
			else
			{
				if (vx < 0)
				{
					vx += speed * 3 * dt;
					if (vx > 0)
						vx = 0;
				}
			}
			moving = false;
		}
		if (!onTheGround)
		{
			canJump = false;
		}

		if (PInput::KeyPressed(JUMP) && canJump)
		{
			SoundManager::GetInstance()->PlaySoundW("BigObjectJump.wav");
			vy = -240;
			onTheGround = false;
			canJump = false;
		}

		if (onTheGround && !PInput::KeyPressed(JUMP))
		{
			canJump = true;
		}
		if (PInput::KeyPressed(UP))
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
			if (now - start_gunturn > 150)
			{
				gun_turn = false;
			}
		}
		if (PInput::KeyPressed(SHOOT))
		{
			if (PInput::KeyPressed(DOWN))
			{
				if (now - last_bullet > 300)
				{
					switch (currentWeapon)
					{
					case 1:
						ShootHoming();
						break;
					case 2:
						ShootThunder();
						break;
					case 3:
						ShootRocket();
						break;
					}
				}
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
					//adjust x,y for more realistic firing
					if (state != STATE_SOPHIA_FALL_90 && state != STATE_SOPHIA_IDLE_90 && state != STATE_SOPHIA_JUMP_90 && state != STATE_SOPHIA_MOVE_90) {
						bullet->SetPosition(x + (flipX ? -20 : 20), y - 3.5f);
					}
					else {
						bullet->SetPosition(x - (flipX ? -3.5f : 3.5f), y - 20);
					}
					
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
				}
			}
		}
		StateChange();
		last_flipX = flipX;
		if (onTheGround && PInput::KeyDown(SHIFT) && (GetTickCount64() - switchDelay >= 1000))
		{
			jason = new Jason(JasonCurrentHealth, x, y - 10, this);

			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetPlayer(jason);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(jason);
			state = STATE_SOPHIA_SHIFT;
			start_shift = now;
		}
		if(!switching)
			onTheGround = false;
	}
}

void Sophia::ShootHoming()
{
	if (Homing <= 0)
		return;
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
				if (CollisionSystem::CheckOverlap(enemy, vision))
				{
					if (Homing <= 0)
						return;
					auto bullet = new Sophia_Bullet_Homing(!flipX, dynamic_cast<DynamicObject*>(enemy));
					bullet->SetPosition(x, y);
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
					Homing--;
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

void Sophia::ShootThunder()
{
	if (Thunder <= 0)
		return;
	if ((new Sophia_Bullet_Thunder)->count > 0)
		return;
	auto bullet = new Sophia_Bullet_Thunder(true, nullptr);
	bullet->SetPosition(x , y + 20);
	Thunder--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
}

void Sophia::ShootRocket()
{
	if (Rocket <= 0)
		return;
	if ((new Sophia_Bullet_Rocket)->count >= 3)
		return;
	int a;
	if (flipX)
	{
		a = -8;
	}
	else
	{
		a = 8;
	}
	auto bullet = new Sophia_Bullet_Rocket(!flipX, 1);
	bullet->SetPosition(x + a, y);
	Rocket--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
	if (Rocket <= 0)
		return;
	auto bullet1 = new Sophia_Bullet_Rocket(!flipX, 2);
	bullet1->SetPosition(x + a, y);
	Rocket--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet1);
	if (Rocket <= 0)
		return;
	auto bullet2 = new Sophia_Bullet_Rocket(!flipX, 3);
	bullet2->SetPosition(x + a, y);
	Rocket--;
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet2);
	last_bullet = GetTickCount();
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
	if (!moving && gun_up == 45)
	{
		if (state != STATE_SOPHIA_MOVE_45)
		{
			dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
			state = STATE_SOPHIA_MOVE_45;
		}
		return;
	}
	if (!moving && gun_up == 90)
	{
		dynamic_cast<Animator_Sophia*>(animator)->isResetFrame = true;
		state = STATE_SOPHIA_IDLE_90;
		return;
	}
	if (!moving && gun_up == 0)
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
	if (animator == NULL)
	{
		animator = new Animator_Sophia();
	}

	if (state == STATE_SOPHIA_DIE)
	{
		animator->Draw(state, x, y - 4, flipX);
		return;
	}
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
		if(state == STATE_SOPHIA_SHIFT)
			animator->Draw(state, sx, sy-2, flipX);
		else
			animator->Draw(state, sx, sy, flipX);	
	}
	else
	{
		if (state == STATE_SOPHIA_SHIFT_IN)
		{
			animator->Draw(state - 1, sx, sy - 2, flipX);
		}
		else
		{
			animator->Draw(state + dynamic_cast<Animator_Sophia*>(animator)->wheel - 1, sx, sy, flipX, 0, damageColor[currentColor]);
		}
	}
}

void Sophia::TakeDamage(int dmg)
{
	if (invincible <= 0)
	{
		this->HP -= dmg;
		SoundManager::GetInstance()->PlaySoundW("JasonGotHit_Outside.wav");
		/*if (state == STATE_SOPHIA_IDLE || state == STATE_SOPHIA_IDLE_90)
			vx = -150;*/
		invincible = 500;
		if (HP < 0)
		{
			HP = 0;
		}

		DebugOut(L"%d\n", HP);
	}
}

bool Sophia::IsDead()
{
	if (HP == 0)
	{
		if(state == 20000) // state 20000: sohpia die and finish play death's animation
		{
			return true;
		}
		return false;
	}
	return false;
}

bool Sophia::isInvincible()
{
	if (invincible <= 0)
		return false;
	else
		return true;
}

void Sophia::Awake(int JasonHealth) {
	SoundManager::GetInstance()->PlaySoundW("swapSophiaAndJason.wav");
	state = STATE_SOPHIA_SHIFT;
	switchDelay = GetTickCount64();
	JasonCurrentHealth = JasonHealth;
	state = STATE_SOPHIA_SHIFT_IN;
	start_shift = GetTickCount();
	onTheGround = true;
}