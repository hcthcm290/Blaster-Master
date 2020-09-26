#include "Jumper.h"

void Jumper::Update()
{
}

void Jumper::Render()
{
	animations["moving"]->Render(x, y);
}
