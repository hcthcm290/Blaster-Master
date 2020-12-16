#include "LifeShowner.h"

LifeShowner::LifeShowner()
{
	this->textRenderer.LoadFont("Font\\LifeLeftFont.txt");
}

void LifeShowner::Update(float dt)
{
	if (DInput::KeyDown(DIK_RETURN))
	{
		auto scene_stack = CGame::GetInstance()->GetSceneStack();

		scene_stack.Pop(); // pop the current scene (life left scene) outof stack
		int target_scene_id = scene_stack.Pop(); // get the actual play scene

		CGame::GetInstance()->SoftSwitchScene(target_scene_id, true, true);
	}
}

void LifeShowner::Render()
{
	this->textRenderer.Render(-27, -10, "LEFT 6");
}
