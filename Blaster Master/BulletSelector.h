#pragma once
#include "StaticObject.h"

class BulletSelector : public StaticObject
{
protected:
	const float blinkingInterval = 0.2;
	const D3DXVECTOR2 listPosition[3] = {
		D3DXVECTOR2(0,0),
		D3DXVECTOR2(0,0),
		D3DXVECTOR2(0,0)
	};
	const float offsetX = 20; // offset from the centere to 2 side of selector, it look like the space between the dot and the '(' ( . )

protected:
	float TimeSinceLastBlink = 0.0;
	bool showTheSelector = true;
	int currentPositionIndex = 0;

public:
	void Update(float dt);
	void Render();
};