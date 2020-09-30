#include "Jumper.h"

void Jumper::Update(DWORD dt) 
{
}

void Jumper::Render()
{
	animator.Draw(1002, x, y, false);
}
