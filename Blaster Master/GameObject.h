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

	static unordered_map<string, CAnimationInfo*> animationInfos;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }
	static void AddAnimation(string id, CAnimationInfo* animInfo);

	CGameObject();

	void Update(DWORD dt);
	void Render();
	~CGameObject();
};