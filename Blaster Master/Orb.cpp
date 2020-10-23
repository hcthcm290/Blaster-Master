#include "Orb.h"

void Orb::Update(DWORD dt)
{

}

void Orb::Render()
{
	animator->Draw(1001, x, y, false);
}
