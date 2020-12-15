#pragma once
#include "StaticObject.h"
#include "TextRenderer.h"

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

	TextRenderer textRenderer;
	const D3DXVECTOR2 textOffset = D3DXVECTOR2(-5, 13); // offset from the list Position
protected:
	float TimeSinceLastBlink = 0.0;
	bool showTheSelector = true;
	int currentPositionIndex = 0;

public:
	WeaponSelector();
	void Update(float dt);
	void Render();
};