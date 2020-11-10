#include "Worm.h"
#include "PlayScene.h"
#include "Debug.h"

Worm::Worm()
{
	//boolean
	flipX = false;
	//animator
	animator->AddAnimation(20502);
}

FRECT Worm::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 18 / 2;
	colRect.right = this->x + 18 / 2;
	colRect.top = this->y - 4 / 2;
	colRect.bottom = this->y + 6 / 2;
	return colRect;
}

void Worm::OnCollisionEnter(CollisionEvent e)
{

}

void Worm::Update(float dt)
{
	vy += 300 * dt;
	float player_x = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x;
	if (player_x < this->x - 18 / 2)
	{
		vx = -50;
		flipX = false;
	}
	else if (player_x > this->x + 18 / 2)
	{
		vx = 50;
		flipX = true;
	}
}

void Worm::Render()
{
	animator->Draw(20502, x, y, flipX);
}