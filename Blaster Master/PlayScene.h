#pragma once
#include "Game.h"
#include "Scene.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "BigGate.h"

#define MAP_BLOCK_WIDTH 256
#define MAP_BLOCK_HEIGHT 256

class CPlayScene : public CScene
{
protected:
	DynamicObject* player;

	unordered_map<int, vector<CGameObject*>> sceneObjects;

	vector<CGameObject*> onScreenObjs;

	StaticObject* mapBackground;

	BigGate* gate;
	float countingTime1;
	float countingTime2;
	bool shiftingCamera = false;
	float deltaShift = 0;
	float totalShifting = 0;

	int state = State::_PLAYSCENE_FREE_PLAYING_;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	virtual void ApllyVelocityToGameObjs(float dt);

	int GetMapBlockID(float x, float y);
	std::vector<int> GetMapBlockID(CGameObject* object);
	vector<CGameObject*> UpdateOnScreenObjs();

public:
	CPlayScene(int id, LPCWSTR filePath);
	void AddGameObjectToScene(CGameObject* obj);
	void RemoveGameObjectFromScene(CGameObject* obj);
	vector<CGameObject*> GetOnScreenObjs();
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	DynamicObject* GetPlayer() { return player; }
	void SetPlayer(DynamicObject* newPlayer) { player = newPlayer; }

	void SwitchSection(BigGate* gate);
};