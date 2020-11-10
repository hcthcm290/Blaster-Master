
#include "DInput.h"
#include "Animator.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Floater_Bullet.h"
#include "Orb.h"
#include "Explosive.h"
Floater_Bullet::Floater_Bullet()
{
	animator = new Animator();
	animator->AddAnimation(floaterBullet);
	state = floaterBullet;
}
void Floater_Bullet::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) || e.pGameObject== dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer())
		boom = true;
}
void Floater_Bullet::Update(float dt)
{
	if (boom)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		DynamicObject*	obj;
		obj = new Explosive();
		obj->SetPosition(x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
	}
}

void Floater_Bullet::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Floater_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 2;
	colRect.right = x + 2;
	colRect.top = y - 2;
	colRect.bottom = y + 2;

	return colRect;
}
