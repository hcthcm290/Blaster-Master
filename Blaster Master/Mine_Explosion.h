#pragma once
#include "DynamicObject.h"

class Mine_Explosion : public DynamicObject
{
private:
	bool active = true;

public:
	Mine_Explosion();
	void Update(float dt);
	void Render();
};