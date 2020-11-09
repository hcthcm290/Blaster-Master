
#include "DInput.h"
#include "Animator.h"
#include "ColliableBrick.h"
#include "Debug.h"
#include "Floater_Bullet.h"
#include "Orb.h"
Floater_Bullet::Floater_Bullet()
{
	animator = new Animator();
	animator->AddAnimation(floaterBullet);
	state = floaterBullet;
}
void Floater_Bullet::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL)
	{
		boom = true;
	}

	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL && e.nx != 0)
	{
 		boom = true;
	}
}
void Floater_Bullet::Update(float dt)
{
	if (boom)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void Floater_Bullet::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Floater_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 4;
	colRect.right = x + 4;
	colRect.top = y - 4;
	colRect.bottom = y + 4;

	return colRect;
}
