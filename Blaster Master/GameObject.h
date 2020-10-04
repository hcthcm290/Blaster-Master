#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"

using namespace std;


class CGameObject
{
protected:
	float x;
	float y;

	int currentState;

	unordered_map<string, CAnimation*> animations;


public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }

	CGameObject();

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	~CGameObject();
};

typedef CGameObject* LPGAMEOBJECT;