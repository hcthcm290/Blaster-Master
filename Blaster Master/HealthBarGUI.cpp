#include "HealthBarGUI.h"
#include "PlayScene.h"
#include "Camera.h"

HealthBarGUI::HealthBarGUI()
{
	offset.x = -107;
	offset.y = 49;
}

void HealthBarGUI::Update(float dt)
{
	DynamicObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	maxHP = player->GetMaxHP();
	currentHP = player->GetCurrentHP();
}

void HealthBarGUI::Render()
{
	D3DXVECTOR3 cameraPosition = Camera::GetInstance()->GetPosition();
	CSprites::GetInstance()->Get(16000)->Draw(cameraPosition.x + offset.x, cameraPosition.y + offset.y);
}
