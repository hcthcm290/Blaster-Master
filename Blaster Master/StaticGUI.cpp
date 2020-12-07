#include "StaticGUI.h"
#include "Camera.h"

StaticGUI::StaticGUI(int spriteID, D3DXVECTOR2 offset)
{
	this->spriteID = spriteID;
	this->offset = offset;
}

void StaticGUI::Update(float dt)
{
}

void StaticGUI::Render()
{
	D3DXVECTOR3 cameraPosition = Camera::GetInstance()->GetPosition();
	CSprites::GetInstance()->Get(spriteID)->Draw(cameraPosition.x + offset.x, cameraPosition.y + offset.y);
}
