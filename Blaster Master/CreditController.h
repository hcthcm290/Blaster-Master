#pragma once
#include "StaticObject.h"
#include "TextRenderer.h"

class CreditController : public StaticObject
{
private:
	TextRenderer textRenderer;
	D3DXVECTOR2 textPosition = D3DXVECTOR2(-10, 120);
	float textMovingSpeed = 15;
	float textLivingTime = 44;

	float CountTime = 0;

	int IDTheEndText = 112000;
	D3DXVECTOR2 TheEndTextPosition = D3DXVECTOR2(40, -40);

public:
	CreditController();
	void Update(float dt);
	void Render();
};