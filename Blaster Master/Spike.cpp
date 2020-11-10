#include "Spike.h"
#include "ColliableBrick.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"
#include "Sprites.h"

Spike::Spike(int x)
{
	if (x<0) 	int flipRotation = -180;
	state = spikeSprite;
	this->SetPosition(x, y);
}

FRECT Spike::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 8;
	colRect.right = this->x + 8;
	colRect.top = this->y - 8;
	colRect.bottom = this->y + 8;
	return colRect;
}

void Spike::OnCollisionEnter(CollisionEvent e)
{

}

void Spike::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (CollisionSystem::CheckOverlap(this, player))
	{
		DebugOut(L"hithithtihtihtih");
	}
}

void Spike::Render()
{
	CSprites::GetInstance()->Get(spikeSprite)->Draw(x, y, false, flipRotation);
}


