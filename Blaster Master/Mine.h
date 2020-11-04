#pragma once
#include "DynamicObject.h"

class Mine : public DynamicObject
{
public:
	Mine();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();
};

