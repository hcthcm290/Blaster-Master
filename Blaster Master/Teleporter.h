#pragma once
#include "DynamicObject.h"
class Teleporter :
    public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};

