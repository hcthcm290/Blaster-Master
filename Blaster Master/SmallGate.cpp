#include "SmallGate.h"
#include "DInput.h"
#include "Jason.h"
#include "CollisionSystem.h"
#include "PlayScene.h"

FRECT SmallGate::GetCollisionBox()
{
	float width = 10;
	float height = 16;

	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void SmallGate::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (DInput::KeyPress(DIK_DOWN) &&
		dynamic_cast<Jason*>(player) != NULL &&
		CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SwitchScene(this->id_target_scene);
	}
}
