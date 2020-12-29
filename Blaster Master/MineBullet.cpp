#include "MineBullet.h"
#include "Game.h"
#include "PlayScene.h"
#include "CollisionSystem.h"
#include "Playable.h"

MineBullet::MineBullet()
{
	animator->AddAnimation(21301);
}


void MineBullet::Update(float dt)
{
	vy += 300 * dt;

	// CHECK IF BULLET IS OVERLAPPED WITH PLAYER //
	///////////////////////////////////////////////
	// The reason we dont use Collision System because the bullet when created may 
	// already overlap with player

	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		// TODO //
		// Set Dmg and push back player //
		if (!dynamic_cast<Playable*>(player)->IsInvulnerable())
		{
			dynamic_cast<DynamicObject*>(player)->TakeDamage(10);
			// Remove bullet from scene //
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		}
	}
}

void MineBullet::Render()
{
	if (vx > 0)
	{
		animator->Draw(21301, x, y, true);
	}
	else
	{
		animator->Draw(21301, x, y, true);
	}
}


FRECT MineBullet::GetCollisionBox()
{
	int width = 10;
	int height = 10;
	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}


