#include "Spike_Interior.h"
#include "ColliableBrick.h"
#include "Utils.h"
#include "DInput.h"
#include "Debug.h"
#include "Sprites.h"
#include "PlayScene.h"

Spike_Interior::Spike_Interior(int w,int h)
{
	this->width = w;
	this->height = h;
}

FRECT Spike_Interior::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = this->x - 8;
	colRect.right = this->x + 8 + 16 * (width - 1);
	colRect.top = this->y - 8;
	colRect.bottom = this->y + 8 + 16 * (height - 1);
	return colRect;
}

void Spike_Interior::OnCollisionEnter(CollisionEvent e)
{
}

void Spike_Interior::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(20);
	}
}

void Spike_Interior::Render()
{
	CSprites::GetInstance()->Get(State::_SPIKE_)->Draw(x, y, false);	
}


