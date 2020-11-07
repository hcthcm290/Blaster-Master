#include "MineBullet.h"

MineBullet::MineBullet()
{
	animator->AddAnimation(21301);
}


void MineBullet::Update(float dt)
{
	vy += 100 * dt;
}

void MineBullet::Render()
{
	if (vx > 0)
	{
		animator->Draw(21301, x, y, true);
	}
	else
	{
		animator->Draw(21301, x, y, true);
	}
}


FRECT MineBullet::GetCollisionBox()
{
	int width = 10;
	int height = 10;
	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void MineBullet::OnCollisionEnter(CollisionEvent e)
{
	// TODO //
	// IF THIS IS A PLAYER, KICK HIS ASS //

	// DELETE BULLET FROM SCENE //

}


