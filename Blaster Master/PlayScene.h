#pragma once
#include "Game.h"
#include "Scene.h"
#include "DynamicObject.h"
#include "StaticObject.h"


class CPlayScene : public CScene
{
protected:
	DynamicObject* player;

	vector<CGameObject*> objects;
	vector<StaticObject> map;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	DynamicObject* GetPlayer() { return player; }
};