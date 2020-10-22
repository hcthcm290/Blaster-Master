#pragma once
#include "DynamicObject.h"

class Dome : public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};