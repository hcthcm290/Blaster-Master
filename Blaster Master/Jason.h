#pragma once
#include "DynamicObject.h"

class Jason : public DynamicObject
{
private:
	int state;
public:
	Jason();
	void Update(DWORD dt);
	void Render();
};