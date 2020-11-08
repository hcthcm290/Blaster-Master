
#include "DInput.h"
#include "Animator_Floater.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Floater.h"
#include "Orb.h"
#include "Floater_Bullet.h"
Floater::Floater()
{
	animator = new Animator_Floater();
	animator->AddAnimation(floaterFly);
	animator->AddAnimation(floaterIdle);
	state = floaterIdle;
	vx = vy = 0;
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
	float distance = min(abs(x - Character_X),abs(y-Character_Y));
	if (distance <= 100)
	{
		this->state = floaterFly;
		if (trigger)
		{
			vx = -floaterSpeed;
			vy = floaterSpeed;
			trigger = false;
			CGameObject* obj = NULL;
			obj = new Floater_Bullet(Character_X, Character_Y);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);

		}
		else
		{
			
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
