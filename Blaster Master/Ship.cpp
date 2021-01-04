
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
	float distance = min(abs(x - Character_X), 200);

	if (x - 200 <= 0.000001)
		trigger = true;
	if (distance < 200)
		trigger = true;
	if (y < Character_Y)
		canShot = true;
	else
	{
		canShot = false;
		waitForShot = 2;
	}
	if (trigger)
	{
		this->state = shipFly;
		if (fly)
		{
			vx = -80;
			fly = false;
		}
		waitForShot += dt;
		mini_waitForShot += dt;
		if (waitForShot >= 2 && canShot)
		{
			shotCount = 4;
			waitForShot = 0;
			mini_waitForShot = 0;
			canShot = false;
		}
		if (mini_waitForShot >= 0.3 && shotCount > 0)
		{
#pragma region Ship_Bullet
			DynamicObject* obj = NULL;
			obj = new Floater_Bullet();
			obj->SetPosition(x, y);
			float modulo = sqrt(pow(Character_X - x, 2) + pow(Character_Y - y, 2));
			float direction_X = float(Character_X - x) / modulo;
			float direction_Y = float(Character_Y - y) / modulo;
			obj->SetVelocity(direction_X * 100, direction_Y * 100);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
#pragma endregion
			shotCount--;
			mini_waitForShot = 0;
		}
	}
}

void Ship::Render()
{
	animator->Draw(state, x, y, !flip);
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
