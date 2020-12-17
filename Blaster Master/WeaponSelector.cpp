 #include "WeaponSelector.h"
#include "DInput.h"
#include "TheEye.h"
#include "Game.h"
#include "Debug.h"
#include "SoundManager.h"
WeaponSelector::WeaponSelector()
{
	textRenderer.LoadFont("Font\\ChooseWeaponFont.txt");
}

void WeaponSelector::Update(float dt)
{
	// -1 because sophia weapon index start from 1
	this->currentPositionIndex = TheEye::GetInstance()->GetSophia()->GetCurrentWeapon() - 1;

	TimeSinceLastBlink += dt;
	if (TimeSinceLastBlink >= blinkingInterval)
	{
		showTheSelector = !showTheSelector;
		TimeSinceLastBlink = 0;
	}

	if (DInput::KeyDown(DIK_LEFT))
	{
		currentPositionIndex = currentPositionIndex - 1;
		DebugOut(L"Left press\n");
	}
	if (DInput::KeyDown(DIK_RIGHT))
	{
		currentPositionIndex = currentPositionIndex + 1;
	}

	if (currentPositionIndex < 0) currentPositionIndex = 0;
	if (currentPositionIndex > 2) currentPositionIndex = 2;

	// the same why we need -1 when get
	TheEye::GetInstance()->GetSophia()->SetCurrentWeapon(this->currentPositionIndex + 1);

	if (DInput::KeyDown(DIK_RETURN))
	{
		Stack<int> scene_stack = CGame::GetInstance()->GetSceneStack();
		scene_stack.Pop(); // the first pop is to eliminate the select weapon scene
		int target_scene_id = scene_stack.Pop();
		CGame::GetInstance()->SwitchScene(target_scene_id);
	}
}

void WeaponSelector::Render()
{
	if (showTheSelector)
	{
		D3DXVECTOR2 currentPos = listPosition[currentPositionIndex];
		CSprites::GetInstance()->Get(spriteID)->Draw(currentPos.x - offsetX, currentPos.y);
		CSprites::GetInstance()->Get(spriteID)->Draw(currentPos.x + offsetX, currentPos.y, true);
	}

	// render the number of weapon left
	for (int i = 0; i < 3; i++)
	{
		textRenderer.Render(listPosition[i].x + textOffset.x, listPosition[i].y + textOffset.y, "20");
	}
}
