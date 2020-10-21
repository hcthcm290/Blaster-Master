#pragma once
#include "DynamicObject.h"
class Insect : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

