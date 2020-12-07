#include "GunBarGUI.h"
#include "TheEye.h"
#include "Camera.h"

GunBarGUI::GunBarGUI()
{
	offset.x = -107;
	offset.y = -15;
}

void GunBarGUI::Update(float dt)
{
	gunPower = TheEye::GetInstance()->GetBigJason()->getGunPower();
}

void GunBarGUI::Render()
{
	D3DXVECTOR3 cameraPosition = Camera::GetInstance()->GetPosition();

	// we need -1 because gunpower start at lv 1
	CSprites::GetInstance()->Get(16200 + gunPower - 1)->Draw(cameraPosition.x + offset.x, cameraPosition.y + offset.y);
}
