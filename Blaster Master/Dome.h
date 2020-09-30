#pragma once
#include "DynamicObject.h"

class Dome : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};