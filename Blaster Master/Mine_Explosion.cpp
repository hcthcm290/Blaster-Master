#include "Mine_Explosion.h"
#include "Animator_Mine_Explosion.h"

Mine_Explosion::Mine_Explosion()
{
	animator = new Animator_Mine_Explosion();
	animator->AddAnimation(21401);
}

void Mine_Explosion::Update(float dt)
{
	if (dynamic_cast<Animator_Mine_Explosion*>(animator)->isDone())
	{
		// remove itself from the Scene
		active = false;
	}
}

void Mine_Explosion::Render()
{
	if (active)
	{
		animator->Draw(21401, x, y, false);
	}
}
