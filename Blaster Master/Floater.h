#pragma once
#include "DynamicObject.h"

class Floater : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};