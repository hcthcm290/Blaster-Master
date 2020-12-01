
#include "Animator.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Canon_Bullet.h"
#include "Explosive.h"

Canon_Bullet::Canon_Bullet()
{
	animator = new Animator();
	animator->AddAnimation(canonBullet);
	state = canonBullet;
}
void Canon_Bullet::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) || e.pGameObject == dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer())
		boom = true;
}
void Canon_Bullet::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(10);
	}
	if (boom)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		DynamicObject* obj;
		obj = new Explosive();
		obj->SetPosition(x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
	}
}

void Canon_Bullet::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Canon_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 4;
	colRect.right = x + 4;
	colRect.top = y - 4;
	colRect.bottom = y + 4;

	return colRect;
}