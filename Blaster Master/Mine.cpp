#include "Mine.h"
#include "Orb.h"
#include "Mine_Explosion.h"
#include "Game.h"
#include "PlayScene.h"
#include "Utils.h"
#include "MineBullet.h"
#include "SoundManager.h"
#include "Explosive.h"
#include "Playable.h"
#include "PlayerBullet.h"
#include "PlayerItem.h"

Mine::Mine()
{
	//set HP
	HP = 30;
	
	animator->AddAnimation(20301);
	animator->AddAnimation(20302);
	animator->AddAnimation(21401); // Mine Explosion
}

void Mine::Update(float dt)
{

}

void Mine::Render()
{

	animator->Draw(20302, x, y, true);
}

FRECT Mine::GetCollisionBox()
{
	int width = 15;
	int height = 9;

	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void Mine::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<Playable*>(e.pGameObject) != NULL || dynamic_cast<PlayerBullet*>(e.pGameObject))
	{
		// Create explosion animation
		auto explosion = new Mine_Explosion();
		explosion->SetPosition(x, y);

		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);

		// Create bullet
		// random between 2 and 4
		int numberOfBullet = rand() % 3 + 2;

		for (int i = 0; i < numberOfBullet; i++)
		{
			float vx_factor = RandomFloat(-2, 2);

			float vy_factor = RandomFloat(3, 4.5);

			CGameObject* bullet = new MineBullet();
			bullet->SetPosition(x, y);

			float baseVelocity = dynamic_cast<MineBullet*>(bullet)->getBaseVelocity();
			dynamic_cast<DynamicObject*>(bullet)->SetVelocity(vx_factor * baseVelocity, -vy_factor * baseVelocity);

			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
		}

		// TODO //
		// Set dmg to player if player touched it //
		if (dynamic_cast<Playable*>(e.pGameObject) != NULL)
		{
			dynamic_cast<DynamicObject*>(e.pGameObject)->TakeDamage(damage);
		}

		// Remove mine from scene //
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);

		// Play Explosion Sound //
		SoundManager::GetInstance()->PlaySoundW("MineWaiting.wav");
	}
}

void Mine::TakeDamage(int dmg)
{
	this->HP -= dmg;
	if (HP < 0)
	{
		HP = 0;
		SoundManager::GetInstance()->PlaySoundW("MineExplosion.wav");
	}
	if (HP == 0)
	{
		srand((int)time(0));
		if ((rand() % 3) == 0)
		{
			PlayerItem* newPlayerItem = new PlayerItem(Power);
			newPlayerItem->SetPosition(x, y);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
		}
		auto explode = new Explosive();
		explode->SetPosition(x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
		// Create bullet
	// random between 2 and 4
		int numberOfBullet = rand() % 3 + 2;

		for (int i = 0; i < numberOfBullet; i++)
		{
			float vx_factor = RandomFloat(-2, 2);

			float vy_factor = RandomFloat(3, 4.5);

			CGameObject* bullet = new MineBullet();
			bullet->SetPosition(x, y);

			float baseVelocity = dynamic_cast<MineBullet*>(bullet)->getBaseVelocity();
			dynamic_cast<DynamicObject*>(bullet)->SetVelocity(vx_factor * baseVelocity, -vy_factor * baseVelocity);

			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
		}

		// TODO //
		// Set dmg to player if player touched it //

		// Remove bullet from scene //
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
}
