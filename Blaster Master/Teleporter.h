#pragma once
#include "DynamicObject.h"
class Teleporter :
    public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

