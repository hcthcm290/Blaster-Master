#include "Mine.h"
#include "Orb.h"
#include "Mine_Explosion.h"
#include "Game.h"
#include "PlayScene.h"

Mine::Mine()
{
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
	if (dynamic_cast<Orb*>(e.pGameObject) != NULL)
	{
		auto explosion = new Mine_Explosion();
		explosion->SetPosition(x, y);

		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);

		active = false;
		count++;
	}
}
