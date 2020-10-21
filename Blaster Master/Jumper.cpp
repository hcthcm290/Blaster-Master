#include "Jumper.h"

void Jumper::Update(DWORD dt)
{
}

void Jumper::Render()
{
	animator.Draw(20102, x, y, false);
}
