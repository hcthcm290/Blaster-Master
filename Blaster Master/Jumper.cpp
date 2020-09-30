#include "Jumper.h"

void Jumper::Update()
{
}

void Jumper::Render()
{
	animator.Draw(1002, x, y, false);
}
