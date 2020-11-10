#include "BigGate.h"
#include "PlayScene.h"

FRECT BigGate::GetCollisionBox()
{
	float width = 16;
	float height = 16;
	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void BigGate::OnCollisionEnter(CollisionEvent e)
{
	if (e.pGameObject == dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer())
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SwitchSection(this);
	}
}
