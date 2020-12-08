#pragma once
#include "DynamicObject.h"
#include "Scene.h"

class MovieScene : public CScene
{
protected:
	int MovieAnimationID;
	int idNextScene; // if timeout, nextScene will be play
	int idBreakOutScene; // if this scene is set, player can press Enter to go to that scene
	int BackgroundAnimationID;

	float timeout;
	float countTime = 0.0f;

protected:
	virtual void _ParseSection_TEXTURES(std::string line);
	virtual void _ParseSection_SPRITES(std::string line);
	virtual void _ParseSection_ANIMATIONS(std::string line);
	virtual void _ParseSection_OBJECTS(std::string line);

public:
	MovieScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Unload();
	virtual void Update(DWORD dt);
	virtual void Render();
};