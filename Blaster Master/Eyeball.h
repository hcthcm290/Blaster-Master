#pragma once
#include "DynamicObject.h"
class Eyeball :
    public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};

