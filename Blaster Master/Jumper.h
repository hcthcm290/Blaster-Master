#pragma once
#include "DynamicObject.h"

class Jumper : public DynamicObject
{
public:
	void Update(float dt);
	void Render();
	D3DRECT GetCollisionBox();
};