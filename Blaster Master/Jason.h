#pragma once
#include "DynamicObject.h"

class Jason : public DynamicObject
{
private:
	int state;
public:
	Jason();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox() { return FRECT(); }
};