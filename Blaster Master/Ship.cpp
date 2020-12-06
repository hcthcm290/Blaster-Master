
#include "DInput.h"
#include "Animator.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Ship.h"
#include "Floater_Bullet.h"

Ship::Ship()
{
	animator = new Animator();
	animator->AddAnimation(shipIdle);
	animator->AddAnimation(shipFly);
	state = shipIdle;
	vx = vy = 0;
	trigger = false;
}
void Ship::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		if (e.nx < 0)
		{
			vx = -100;
			flip = true;
		}
		if (e.nx > 0)
		{
			vx = 100;
			flip = false;
		}
	}
}

void Ship::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(8);
	}

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
		this->state = shipFly;
		if (fly)
		{
			vx = -100;
			fly = false;
		}
		waitForShot += dt;
		mini_waitForShot += dt;
		if (waitForShot >= 2)
		{
			shotCount = 4;
			waitForShot = 0;
			mini_waitForShot = 0;
		}
		if (mini_waitForShot >= 0.2 && shotCount > 0)
		{
#pragma region Ship_Bullet
			DynamicObject* obj = NULL;
			obj = new Floater_Bullet();
			obj->SetPosition(x, y);
			float module = sqrt(pow(Character_X - x, 2) + pow(Character_Y - y, 2));
			float direction_X = float(Character_X - x) / module;
			float direction_Y = float(Character_Y - y) / module;
			obj->SetVelocity(direction_X * 150, direction_Y * 150);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
#pragma endregion
			shotCount--;
			mini_waitForShot = 0;
		}
	}
}

void Ship::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Ship::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 10;
	colRect.right = x + 10;
	colRect.top = y - 6;
	colRect.bottom = y + 6;

	return colRect;
}
