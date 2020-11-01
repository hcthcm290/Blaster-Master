#pragma once
#include "DynamicObject.h"
class Worm : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

