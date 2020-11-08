
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
Floater_Bullet::Floater_Bullet(float c_x, float c_y)
{
	animator = new Animator();
	animator->AddAnimation(floaterBullet);
	state = floaterBullet;
	vx = c_x/100, vy = c_y/100;
}
void Floater_Bullet::OnCollisionEnter(CollisionEvent e)
{

}
void Floater_Bullet::Update(float dt)
{
	
}

void Floater_Bullet::Render()
{
	animator->Draw(state, x, y, flip);
}

FRECT Floater_Bullet::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 3;
	colRect.right = x + 3;
	colRect.top = y - 3;
	colRect.bottom = y + 3;

	return colRect;
}
