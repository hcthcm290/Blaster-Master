#include "Canon.h"
#include "Animator_Canon.h"
#include "Canon_Bullet.h"
#include "Debug.h"
#include "SoundManager.h"
#include "InteriorScene.h"

Canon::Canon()
{
	HP = 50;
	animator = new Animator_Canon();
	animator->AddAnimation(canon);
	animator->AddAnimation(canonHor);
	animator->AddAnimation(canonVer);
	this->state = canon;
}
void Canon::Update(float dt)
{
	CGameObject* player = dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(8);
	}
	if (waitForShot >= 0.6)
	{
		if (verFirst)
		{
			//add bullet vertical
			this->state = canonVer;
			verFirst = !verFirst; 
			#pragma region Canon_Bullet
				DynamicObject* obj1,*obj2;
				obj1 = new Canon_Bullet();
				obj1->SetPosition(x, y);
				obj1->SetVelocity(0,-300);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj1);
				obj2 = new Canon_Bullet();
				obj2->SetPosition(x, y);
				obj2->SetVelocity(0, 300);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj2);
			#pragma endregion
		}
		else
		{
			//add bullet horizontal
			this->state = canonHor;
			verFirst = !verFirst;
			#pragma region Canon_Bullet
				DynamicObject* obj1,*obj2;
				obj1 = new Canon_Bullet();
				obj1->SetPosition(x, y);
				obj1->SetVelocity(-300,0);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj1);
				obj2 = new Canon_Bullet();
				obj2->SetPosition(x, y);
				obj2->SetVelocity(300,0);
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj2);
			#pragma endregion
		}
		waitForShot = 0;
		SoundManager::GetInstance()->PlaySoundW("WormMoving_CannonShot_TeleporterShot.wav");
	}
	if (waitForShot>=0.2)
	this->state = canon;
	waitForShot += dt;
}

void Canon::Render()
{
	animator->Draw(state, x, y, false);

}
FRECT Canon::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 14;
	colRect.right = x + 14;
	colRect.top = y - 14;
	colRect.bottom = y + 14;

	return colRect;
}