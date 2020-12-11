#pragma once
#include "GameObject.h"
#include "Scene.h"

class BasicScene : public CScene
{
protected:
	std::vector<CGameObject*> listSceneObject;
	int BackgroundSpriteID;

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