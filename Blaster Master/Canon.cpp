#include "Canon.h"
#include "Animator.h"
#include "Canon_Bullet.h"


Canon::Canon()
{
	animator = new Animator();
	animator->AddAnimation(canon);
	animator->AddAnimation(canonHor);
	animator->AddAnimation(canonVer);
	this->state = canon;
}
void Canon::Update(float dt)
{
	if (waitForShot >= 1)
	{
		if (verFirst)
		{
			//add bullet vertical
			this->state = canonVer;
			verFirst = !verFirst; 
			#pragma region Canon_Bullet
				DynamicObject* obj = NULL;
				obj = new Canon_Bullet();
				obj->SetPosition(x, y);
				obj->SetVelocity(0,-150);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
				obj->SetVelocity(0, 150);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
			#pragma endregion
		}
		else
		{
			//add bullet horizontal
			this->state = canonHor;
			verFirst = !verFirst;
			#pragma region Canon_Bullet
				DynamicObject* obj = NULL;
				obj = new Canon_Bullet();
				obj->SetPosition(x, y);
				obj->SetVelocity(-150,0);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
				obj->SetVelocity(150,0);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
			#pragma endregion
		}
	}
	else
		if (waitForShot >= 0.5)
			this->state = canon;
	waitForShot += dt;
}

void Canon::Render()
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
FRECT Canon::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 13;
	colRect.right = x + 13;
	colRect.top = y - 13;
	colRect.bottom = y + 13;

	return colRect;
}