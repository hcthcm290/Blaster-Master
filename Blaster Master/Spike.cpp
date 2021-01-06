#include "Spike.h"
#include "ColliableBrick.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"
#include "Sprites.h"

Spike::Spike(int length)
{
	this->len = length;
}

FRECT Spike::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 8;
	colRect.right = this->x + 8 + 16 * (len - 1);
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
		dynamic_cast<DynamicObject*>(player)->TakeDamage(20);
		DebugOut(L"Set Damage for Big Jason, this can be use in both scene \n");
	}
}

void Spike::Render()
{

	
		CSprites::GetInstance()->Get(State::_SPIKE_)->Draw(x, y, false);
	
	
}


