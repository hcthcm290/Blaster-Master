#include "StaticObject.h"

void StaticObject::Update(float dt)
{

}

void StaticObject::Render()
{
	CSprites::GetInstance()->Get(spriteID)->Draw(x, y);
}
