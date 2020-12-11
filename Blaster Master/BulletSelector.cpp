#include "BulletSelector.h"
#include "DInput.h"

void BulletSelector::Update(float dt)
{
	TimeSinceLastBlink += dt;
	if (TimeSinceLastBlink >= blinkingInterval)
	{
		showTheSelector = !showTheSelector;
		TimeSinceLastBlink = 0;
	}

	if (DInput::KeyDown(DIK_LEFT))
	{
		currentPositionIndex = (currentPositionIndex - 1) % 3;
	}
	if (DInput::KeyDown(DIK_RIGHT))
	{
		currentPositionIndex = (currentPositionIndex + 1) % 3;
	}
}

void BulletSelector::Render()
{
	if (!showTheSelector) return;

	D3DXVECTOR2 currentPos = listPosition[currentPositionIndex];
	CSprites::GetInstance()->Get(spriteID)->Draw(currentPos.x + offsetX, y);
	CSprites::GetInstance()->Get(spriteID)->Draw(currentPos.x - offsetX, y, true);
}
