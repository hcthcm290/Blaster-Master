#include "BigJason.h"
#include "Animator.h"
#include "PInput.h"
#include "Debug.h"
#include "Game.h"
#include "TheEye.h"

#define I_JASON_IDLE_DOWN 29901
#define I_JASON_IDLE_UP 29902
#define I_JASON_IDLE_SIDE 29903
#define I_JASON_WALK_DOWN 29904
#define I_JASON_WALK_UP 29905
#define I_JASON_WALK_SIDE 29906

#define I_JASON_DIE 29907

BigJason::BigJason()
{
	HP = 40;
	maxHP = HP;
	state = I_JASON_IDLE_DOWN;
	animator = new Animator();
	animator->AddAnimation(29901);
	animator->AddAnimation(29902);
	animator->AddAnimation(29903);
	animator->AddAnimation(29904);
	animator->AddAnimation(29905);
	animator->AddAnimation(29906);
	animator->AddAnimation(29907);

	TheEye::GetInstance()->SetBigJason(this);

	bulletManager->SetLevel(TheEye::GetInstance()->GetJason()->GetBulletPower());
}

void BigJason::Update(float dt)
{
	if (DInput::KeyDown(DIK_P))
	{
		this->bulletManager->SetLevel(this->bulletManager->GetLevel() + 1);
		return;
	}
	//sat thuong
	if (HP == 0)
	{
		vx = 0;
		vy = 0;

		if (state != I_JASON_DIE && state != 20000)
		{
			state = I_JASON_DIE;
			die = GetTickCount();
		}
		else
		{
			if (GetTickCount() - die > 600)
			{
				state = 20000;
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
	if (IsInvulnerable() && currentColor == 0)
	{
		//currentColor = 2;
		currentColor = 4;
		lastDamageTime = now;
	}
	if (IsInvulnerable())
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

	//check 
	bulletManager->CheckBullet();
	bulletManager->CheckCheat();

	//phim bam
	if (PInput::KeyPressed(LEFT) && !PInput::KeyPressed(RIGHT))
	{
		flipX = true;
		vx = -120;
		if(!isUpDown)
			state = I_JASON_WALK_SIDE;
		isSide = true;
	}
	else if (PInput::KeyPressed(RIGHT) && !PInput::KeyPressed(LEFT))
	{
		flipX = false;
		vx = 120;
		if(!isUpDown)
			state = I_JASON_WALK_SIDE;
		isSide = true;
	}
	else
	{
		vx = 0;
		isSide = false;
	}

	if (PInput::KeyPressed(UP) && !PInput::KeyPressed(DOWN))
	{
		vy = -120;
		isUpDown = true;
		if (!isSide)
			state = I_JASON_WALK_UP;
	}
	else if (PInput::KeyPressed(DOWN) && !PInput::KeyPressed(UP))
	{
		vy = 120;
		isUpDown = true;
		if(!isSide)
			state = I_JASON_WALK_DOWN;
	}
	else
	{
		vy = 0;
		isUpDown = false;
	}
	if (vx == 0 && vy == 0)
	{
		if (state >= I_JASON_WALK_DOWN && state != I_JASON_DIE)
		{
			state -= 3;
		}
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
	animator->Draw(state, x, y, !flipX, 0, damageColor[currentColor]);
}

FRECT BigJason::GetCollisionBox()
{
	int width = 20;
	int height = 12;

	// center of collision box is at the shadow of jason
	int deltaShiftY = 12;

	return FRECT(x - width / 2, y + deltaShiftY - height / 2, x + width / 2, y + deltaShiftY + height / 2);
}

bool BigJason::IsInvulnerable()
{
	return false;
}

void BigJason::TakeDamage(int dmg)
{
}

float BigJason::GetEnterGateSpeed()
{
	return 40;
}

void BigJason::NotifySwitchSceneOut()
{
	float BigJasonHealthPercent = (float)GetCurrentHP() / GetMaxHP();
	DynamicObject* jason = TheEye::GetInstance()->GetJason();
	jason->SetCurrentHP(jason->GetMaxHP() * BigJasonHealthPercent);

	dynamic_cast<Jason*>(jason)->SetBulletPower(bulletManager->GetLevel());
}
