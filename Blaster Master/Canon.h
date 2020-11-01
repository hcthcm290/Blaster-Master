#pragma once
#include "DynamicObject.h"
class Canon :
    public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

