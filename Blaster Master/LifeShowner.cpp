#include "LifeShowner.h"
#include "TheEye.h"
#include "Utils.h"

LifeShowner::LifeShowner()
{
	this->textRenderer.LoadFont("Font\\LifeLeftFont.txt");
	timeoutCountdown = 3;
}

void LifeShowner::Update(float dt)
{
	timeoutCountdown -= dt;

	if (timeoutCountdown <= 0)
	{
		auto scene_stack = CGame::GetInstance()->GetSceneStack();

		scene_stack.Pop(); // pop the current scene (life left scene) outof stack
		int target_scene_id = scene_stack.Pop(); // get the actual play scene

		CGame::GetInstance()->SoftSwitchScene(target_scene_id, true, false);
	}
}

void LifeShowner::Render()
{
	this->textRenderer.Render(-27, -10, "LEFT " + IntToString(TheEye::GetInstance()->GetLifeLeft()));
}
