#include "Orb.h"

void Orb::Update(DWORD dt)
{

}

void Orb::Render()
{
	animations["Spinning"]->Render(x, y);
}
