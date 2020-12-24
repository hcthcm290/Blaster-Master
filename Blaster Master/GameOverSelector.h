#pragma once
#include "StaticObject.h"
#include "TextRenderer.h"
#include "Animator.h"

class GameOverSelector : public StaticObject
{
protected:
	const D3DXVECTOR2 listPosition[2] = {
		D3DXVECTOR2(-50, -15),
		D3DXVECTOR2(-50, 9)
	};

	Animator animator;

protected:
	int currentPositionIndex = 0;

public:
	GameOverSelector();
	void Update(float dt);
	void Render();
};