#pragma once
#include "DynamicObject.h"

class Floater : public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};