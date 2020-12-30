#include "StaticObject.h"

void StaticObject::Update(float dt)
{

}

void StaticObject::Render()
{
	CSprites::GetInstance()->Get(spriteID)->Draw(x, y, false, 0, D3DCOLOR_XRGB(255, 255, 255), this->z_index);
}
