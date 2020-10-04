#include "Dome.h"

void Dome::Update(DWORD dt)
{
}

void Dome::Render()
{
	animator.Draw(1003, x, y, false);
}
