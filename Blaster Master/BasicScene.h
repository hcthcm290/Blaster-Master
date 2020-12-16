#pragma once
#include "GameObject.h"
#include "Scene.h"

#define BASICSCENE_INVALID_BACKGROUND_ID -1

class BasicScene : public CScene
{
protected:
	std::vector<CGameObject*> listSceneObject;
	int BackgroundSpriteID = BASICSCENE_INVALID_BACKGROUND_ID;

protected:
	virtual void _ParseSection_TEXTURES(std::string line);
	virtual void _ParseSection_SPRITES(std::string line);
	virtual void _ParseSection_ANIMATIONS(std::string line);
	virtual void _ParseSection_OBJECTS(std::string line);

public:
	BasicScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Unload();
	virtual void Update(DWORD dt);
	virtual void Render();
};