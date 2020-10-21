#pragma once
#include "DynamicObject.h"
class Skull :
    public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

