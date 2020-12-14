#pragma once
#include "StaticObject.h"

class WeaponSelector : public StaticObject
{
protected:
	const float blinkingInterval = 0.02;
	const D3DXVECTOR2 listPosition[3] = {
		D3DXVECTOR2(-32, 60),
		D3DXVECTOR2(3, 60) ,
		D3DXVECTOR2(36, 60)
	};
	const float offsetX = 12; // offset from the centere to 2 side of selector, it look like the space between the dot and the '(' ( . )

protected:
	float TimeSinceLastBlink = 0.0;
	bool showTheSelector = true;
	int currentPositionIndex = 0;

public:
	void Update(float dt);
	void Render();
};