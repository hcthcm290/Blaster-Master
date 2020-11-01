#pragma once
#include "DynamicObject.h"
class Eyeball :
    public DynamicObject
{
public:
	void Update(float dt);
	void Render();
};

