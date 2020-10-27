#include "Jason.h"
#include "Animator_Jason.h"

Jason::Jason() {
	animator = new Animator_Jason();
	state = Constant::_JASON_DIE_;
}

void Jason::Update(DWORD dt)
{

}

void Jason::Render()
{
	animator->Draw(state, x, y, false);
}
