#pragma once
#include "DynamicObject.h"
class Skull :
    public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};

