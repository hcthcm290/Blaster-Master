#pragma once
#include "DynamicObject.h"

class Jumper : public DynamicObject
{
private:
	int state;
	bool keyRelease = true;
	bool flip = false;
	int direction = -1;

public:
	Jumper();
	void Update(DWORD dt);
	void Render();
};