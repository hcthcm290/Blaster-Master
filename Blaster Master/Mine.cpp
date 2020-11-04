#include "Mine.h"
Mine::Mine()
{
	animator->AddAnimation(20301);
	animator->AddAnimation(20302);
}

void Mine::Update(float dt)
{

}

void Mine::Render()
{
	animator->Draw(20302, x, y, true);
}

FRECT Mine::GetCollisionBox()
{
	int width = 15;
	int height = 9;

	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}
