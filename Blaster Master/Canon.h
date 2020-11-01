#pragma once
#include "DynamicObject.h"
class Canon :
    public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};

