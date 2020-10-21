#pragma once
#include "DynamicObject.h"

class Mine : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

