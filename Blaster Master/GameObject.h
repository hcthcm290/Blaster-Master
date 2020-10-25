#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "RECT.h"

using namespace std;


class CGameObject
{
protected:
	float x;
	float y;

	int currentState;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }

	D3DVECTOR GetPosition();

	CGameObject();

	virtual FRECT GetCollisionBox() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	~CGameObject();
};

typedef CGameObject* LPGAMEOBJECT;