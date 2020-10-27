#include "Orb.h"

void Orb::Update(DWORD dt)
{

}

void Orb::Render()
{
	animator->Draw(20902, x, y, false);
}
