#include "Grenade.h"
#include "Animator.h"
#include "Debug.h"
#include "Enemy.h"
#include "ColliableBrick.h"
#include "Explosive_Cannon_Bullet.h"
#include "BigJason.h"
#include "PlayScene.h"
#include "Utils.h"
#include <cstdlib>
#include <ctime>
#include "SoundManager.h"

#pragma region Boom
	class Small_Boom : public CGameObject
	{
		private:
			float livingTime = 0.01;
		public:
			void Update(float dt)
			{
				if (livingTime <= 0)
				{
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
				}
				else
					livingTime -= dt;
			}
			void Render()
			{
				if (livingTime <= 0) return;
				CSprites::GetInstance()->Get(State::_Grenade_Small_Boom)->Draw(x, y, false);
			}
			FRECT GetCollisionBox() {
				return FRECT(x, y , x , y);
			}
			
	};
	class Big_Boom : public CGameObject
	{
		private:
			float livingTime = 0.01;
		public:
			void Update(float dt)
			{
				if (livingTime <= 0)
				{
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
				}
				else
					livingTime -= dt;
			}
			void Render()
			{
				if (livingTime <= 0) return;
				CSprites::GetInstance()->Get(State::_Grenade_Big_Boom)->Draw(x, y, false);
			}
			FRECT GetCollisionBox() {
				return FRECT(x, y, x , y);
			}
	};
	class Grenade_Boom : public CGameObject
	{
		private:
			int boomCount = 3;
			float delay = 0.05;
		public:
			void Update(float dt)
			{
				
				delay -= dt;
				if (boomCount > 0 && delay<=0)
				{
					delay = 0.05;
					boomCount--;
					//randomPosition For Boom
					int randSmallx = rand() % (2 - 1 + 1) + 1;
					int randBigx = rand() % (2 - 1 + 1) + 1;
					int randSmally = rand() % (2 - 1 + 1) + 1;
					int randBigy = rand() % (2 - 1 + 1) + 1;
					float dirSmallx = RandomFloat(-1, 1);
					float dirBigx = RandomFloat(-1, 1);
					float dirSmally = RandomFloat(-1, 1);
					float dirBigy = RandomFloat(-1, 1);
					if (dirSmallx < 0) dirSmallx = -1;
					else dirSmallx = 1;
					if (dirBigx < 0) dirBigx = -1;
					else dirBigx = 1;
					if (dirSmally < 0) dirSmally = -1;
					else dirSmally = 1;
					if (dirBigy < 0) dirBigy = -1;
					else dirBigy = 1;
					//Small Boom
					CGameObject* objSmall = new Small_Boom();
					objSmall->SetPosition(x + dirSmallx * randSmallx * 8, y + dirSmally * randSmally * 8);
					
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(objSmall);
					//Big Boom
					CGameObject* objBig = new Big_Boom();
					objBig->SetPosition(x + dirBigx * randBigx * 8, y + dirBigy * randBigy * 8);
					dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(objBig);
				}
				else
					if (boomCount<=0)	
						dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
			}
			void Render()
			{
				
			}
			FRECT GetCollisionBox() {
				return FRECT(x, y, x, y);
			}
	};
#pragma endregion
Grenade::Grenade(bool check)
{
	this->Horizontal = check;
	this->PlayerY = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y +10;
	SoundManager::GetInstance()->PlaySoundW("Grenade.wav");
}
void Grenade::Remove()
{
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	CGameObject* obj = new Explosive_Cannon_Bullet();
	obj->SetPosition(x, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
	CGameObject* objBoom = new Grenade_Boom();
	objBoom->SetPosition(x , y );
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(objBoom);
}
void Grenade::Update(float dt)
{
	if (livingTime <= 0)
	{
		boom = true;
	}
	else
		livingTime -= dt;
	if (y >= PlayerY && Horizontal)
		boom = true;
	if (boom)
	{
		this->Remove();
	}
}

void Grenade::Render()
{
	if (livingTime <= 0) return;
	CSprites::GetInstance()->Get(State::_Grenade_)->Draw(x, y, false);
}

FRECT Grenade::GetCollisionBox() {
	return FRECT(x + 4, y - 4, x - 4, y + 4);
}

void Grenade::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Enemy*>(e.pGameObject))
	{
		//if (livingTine<=0|| boom) 
		//dynamic_cast<DynamicObject*>(e.pGameObject)->TakeDamage(20);
		
	}
	else
	{
		if (dynamic_cast<ColliableBrick*>(e.pGameObject))
		{
			vx = 0;
			vy = 0;
		}
	}
}