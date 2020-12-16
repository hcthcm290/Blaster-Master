#pragma once
#include "GameObject.h"
#include "TextRenderer.h"
class LifeShowner : public CGameObject
{
private:
	TextRenderer textRenderer;

public:
	LifeShowner();
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox() { return FRECT(); }
};