#include "StaticObject.h"

void StaticObject::Update(DWORD dt)
{
}

void StaticObject::Render()
{
	CSprites::GetInstance()->Get(spriteID)->Draw(x, y);
}
