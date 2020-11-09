#pragma once
#include "Game.h"
#include "Scene.h"
#include "DynamicObject.h"
#include "StaticObject.h"

#define MAP_BLOCK_WIDTH 320
#define MAP_BLOCK_HEIGHT 320

class CPlayScene : public CScene
{
protected:
	DynamicObject* player;

	unordered_map<int, vector<CGameObject*>> sceneObjects;

	vector<CGameObject*> onSCeneObjs;

	StaticObject* mapBackground;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
	int GetMapBlockID(float x, float y);
	vector<CGameObject*> GetOnScreenObjs();

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void ApllyVelocityToGameObjs(float dt);
	virtual void Render();
	virtual void Unload();
	void RemoveGameObjectFromScene(CGameObject* obj);
	void AddGameObjectToScene(CGameObject* obj);
	DynamicObject* GetPlayer() { return player; }
};