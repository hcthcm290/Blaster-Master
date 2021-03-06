#pragma once
#include "DynamicObject.h"
#include "BasicScene.h"

class MovieScene : public BasicScene
{
protected:
	int MovieAnimationID;
	int idNextScene; // if timeout, nextScene will be play
	int idBreakOutScene; // if this scene is set, player can press Enter to go to that scene
	bool canBreakOut = false;

	// first is color
	// second is living time of that color
	vector<pair<D3DXCOLOR, float>> listBackgroundColor;
	D3DXCOLOR defaultBackgroundColor;
	int currentBackgroundColorIndex = -1;
	float lastColorChoosenTime = -1;

	// delay time count from the beginning of the scene
	float backgroundColorDelay;

	// living time of the background color
	float backgroundColorLivingTime;

	float timeout;
	float countTime = 0;

	Animator* movieAnimator;

	D3DXVECTOR2 animationOffset = D3DXVECTOR2(0, 0);

protected:
	virtual void _ParseSection_ListBackgroundColor(std::string line);

public:
	MovieScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Unload();
	virtual void Update(DWORD dt);
	virtual void Render();
};