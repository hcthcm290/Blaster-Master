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
	HP = 50;

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
	if (trigger)
	{
		this->state = orbFly;
		float module = sqrt(pow(Character_X - x, 2) + pow(Character_Y - y, 2));
		float direction_X = float(Character_X - x) / module;
		float direction_Y = float(Character_Y - y) / module;
		this->SetVelocity(direction_X * orbSpeed, direction_Y * orbSpeed);
	}
	

	if (CollisionSystem::CheckOverlap(this, player))
	{
		// TODO //
		// Set Dmg and push back player //
		dynamic_cast<DynamicObject*>(player)->TakeDamage(12);

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
	animator->Draw(state, x, y, false);
}


