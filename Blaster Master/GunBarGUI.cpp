#include "GunBarGUI.h"
#include "TheEye.h"
#include "Camera.h"
#include "Debug.h"

GunBarGUI::GunBarGUI()
{
	offset.x = -107;
	offset.y = -15;
}

void GunBarGUI::Update(float dt)
{
	gunPower = TheEye::GetInstance()->GetBigJason()->getGunPower();

	if (gunPower < 1 || gunPower > 8)
	{
		DebugOut(L"Error with gunpower bar");
	}
}

void GunBarGUI::Render()
{
	D3DXVECTOR3 cameraPosition = Camera::GetInstance()->GetPosition();

	if (gunPower < 1 || gunPower > 8)
	{
		gunPower = TheEye::GetInstance()->GetBigJason()->getGunPower();
	}

	// we need -1 because gunpower start at lv 1
	CSprites::GetInstance()->Get(16200 + gunPower - 1)->Draw(cameraPosition.x + offset.x, cameraPosition.y + offset.y);
}
