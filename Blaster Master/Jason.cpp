#include "Jason.h"
#include "Animator_Jason.h"

Jason::Jason() {
	animator = new Animator_Jason();
	animator->AddAnimation(Constant::_JASON_STAND_);
	animator->AddAnimation(Constant::_JASON_WALK_);
	animator->AddAnimation(Constant::_JASON_JUMP_);
	animator->AddAnimation(Constant::_JASON_CLIMB_);
	animator->AddAnimation(Constant::_JASON_CRAWL_);
	animator->AddAnimation(Constant::_JASON_DIE_);
	state = Constant::_JASON_DIE_;
}

void Jason::Update(float dt)
{

}

void Jason::Render()
{
	animator->Draw(state, x, y, false);
}

FRECT Jason::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 10/2;
	colRect.right = x + 10/2;
	colRect.top = y - 17/2;
	colRect.bottom = y + 17/2;

	return colRect;
}
