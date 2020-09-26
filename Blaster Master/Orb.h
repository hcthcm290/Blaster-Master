#pragma once
#include "DynamicObject.h"

class Orb : public DynamicObject
{
public:
	void Update(DWORD dt);
	void Render();
};