#include "GameOverSelector.h"
#include "Sprites.h"
#include "InteriorScene.h"
#include "IDSceneConstant.h"

GameOverSelector::GameOverSelector()
{
	animator.AddAnimation(211300);
}

void GameOverSelector::Update(float dt)
{
	if (DInput::KeyDown(DIK_DOWN))
		this->currentPositionIndex += 1;

	if (DInput::KeyDown(DIK_UP))
		this->currentPositionIndex -= 1;

	if (this->currentPositionIndex < 0) this->currentPositionIndex = 0;
	if (this->currentPositionIndex > 1) this->currentPositionIndex = 1;

	if (DInput::KeyDown(DIK_RETURN))
	{
		if (this->currentPositionIndex == 0) // continue
		{
			Stack<int> scene_stack = CGame::GetInstance()->GetSceneStack();
			scene_stack.Pop(); // the first pop is to eliminate the select weapon scene
			int target_scene_id = scene_stack.Pop();

			if (dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetScene(target_scene_id)))
			{
				target_scene_id = scene_stack.Pop();
			}

			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetScene(target_scene_id))->HardReload();
			CGame::GetInstance()->SoftSwitchScene(target_scene_id, true, false);
		}
		else if (currentPositionIndex == 1) // end
		{
			CGame::GetInstance()->FlushAllSceneData();

			CGame::GetInstance()->SwitchScene(IDSceneConstant::TITLE_SCENE);
		}
	}
}

void GameOverSelector::Render()
{
	auto position = this->listPosition[this->currentPositionIndex];
	animator.Draw(211300, position.x, position.y, false);
}
