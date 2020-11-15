
#include "DInput.h"
#include "Animator_Floater.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Floater.h"
#include "Orb.h"
#include "Floater_Bullet.h"

Floater::Floater()
{
	//set HP
	HP = 50;

	animator = new Animator_Floater();
	animator->AddAnimation(floaterFly);
	animator->AddAnimation(floaterIdle);
	state = floaterIdle;
	vx = vy = 0;
	trigger = false;
}
void Floater::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject))
	{
		if (e.ny < 0)
		{
			vy = -floaterSpeed;
		}
		if (e.ny > 0 )
		{
			vy = floaterSpeed;
		}
		if (e.nx < 0 )
		{
			vx = -floaterSpeed;
			flip = true;
		}
		if (e.nx > 0 )
		{
			vx = floaterSpeed;
			flip = false;
		}
	}
}

void Floater::Update(float dt)
{
	float Character_X = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	float Character_Y = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y;
	float min_drc_x = min(abs(x - Character_X), 100);
	float min_drc_y = min(abs(y - Character_Y), 100);
	float distance=  min(min_drc_x,min_drc_y);
	
	if (x - 100 <= 0.000001 || y - 100 <= 0.000001)
		trigger = true;
	if ( distance < 100 )
		trigger = true;

	if (trigger)
	{
		this->state = floaterFly;
		if (fly)
		{
			vx = -floaterSpeed;
			vy = floaterSpeed;
			fly = false;
		}
		waitForShot += dt;
		mini_waitForShot += dt;
		if (waitForShot >= 5)
		{
			shotCount = 2;
			waitForShot = 0;
			mini_waitForShot = 0;
		}
		if (mini_waitForShot>=0.5 && shotCount>0)
		{
			#pragma region Floater_Bullet
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

void Floater::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Floater::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 7;
	colRect.right = x + 7;
	colRect.top = y - 6;
	colRect.bottom = y + 6;

	return colRect;
}
