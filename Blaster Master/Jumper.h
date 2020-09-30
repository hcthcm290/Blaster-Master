#pragma once
#include "DynamicObject.h"

class Jumper : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};