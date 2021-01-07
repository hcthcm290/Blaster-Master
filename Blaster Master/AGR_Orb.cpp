#include "AGR_Orb.h"
#include "ColliableBrick.h"
#include "Mine.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"
#include "Explosive.h"

AGR_Orb::AGR_Orb()
{
	//set HP
	HP = 30;

	animator = new Animator();
	animator->AddAnimation(orbIdle);
	animator->AddAnimation(orbFly);
	state = orbIdle;
	vx = vy = 0;
}

FRECT AGR_Orb::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - ORB_COLLISION_BOX_WIDTH / 2;
	colRect.right = this->x + ORB_COLLISION_BOX_WIDTH / 2;
	colRect.top = this->y - ORB_COLLISION_BOX_HEIGHT / 2;
	colRect.bottom = this->y + ORB_COLLISION_BOX_HEIGHT / 2;
	return colRect;
}

void AGR_Orb::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL)
	{
		if (e.nx < 0)
		{
			vx = -orbSpeed;
		}
		if (e.nx > 0)
		{
			vx = orbSpeed;
		}
		fly = false;
	}
}

void AGR_Orb::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float Character_X = player->GetPosition().x;
	float Character_Y = player->GetPosition().y;
	float min_drc_x = min(abs(x - Character_X), 100);
	float min_drc_y = min(abs(y - Character_Y), 100);
	float distance = min(min_drc_x, min_drc_y);

	if (x - 100 <= 0.000001 || y - 100 <= 0.000001)
		trigger = true;
	if (distance < 100)
		trigger = true;
	if ((y - Character_Y) > 30) fly = false;
	if (trigger&& fly)
	{
		this->state = orbFly;
		float modulo = sqrt(pow(Character_X - x, 2) + pow(Character_Y - y, 2));
		float direction_X = float(Character_X - x) / modulo;
		float direction_Y = float(Character_Y - y) / modulo;
		this->SetVelocity(direction_X * orbSpeed, direction_Y * orbSpeed);
	}

	if (CollisionSystem::CheckOverlap(this, player))
	{
		// TODO //
		// Set Dmg and push back player //
		dynamic_cast<DynamicObject*>(player)->TakeDamage(10);

		// Remove bullet from scene //
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		DynamicObject* obj;
		obj = new Explosive();
		obj->SetPosition(x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
	}
}

void AGR_Orb::Render()
{
	if (inv != -1) {
		animator->Draw(state, x, y, false, 0, Color[inv]);
		if (GetTickCount64() - last_blink >= 50) {
			if (GetTickCount64() > startTakeDamage + 150)
			{
				inv = -1;
			}
			else
			{
				last_blink = GetTickCount64();
				switch (inv)
				{
				case 1: inv = 0; break;
				case 0: inv = 1; break;
				}
			}
		}
	}
	else
	{
		animator->Draw(state, x, y, false);
	}
}


