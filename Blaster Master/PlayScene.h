#pragma once
#include "Game.h"
#include "Scene.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "BigGate.h"
#include "ForegroundTile.h"

#define MAP_BLOCK_WIDTH 256
#define MAP_BLOCK_HEIGHT 256

class CPlayScene : public CScene
{
protected:
	DynamicObject* player;
	DynamicObject* playerBackup;

	unordered_map<int, vector<CGameObject*>> sceneObjects;
	unordered_map<int, vector<ForegroundTile*>> foregroundTiles;
	unordered_map<int, vector<CGameObject*>> playableObjects;
	vector<CGameObject*> GUIObjects;

	vector<CGameObject*> onScreenObjs;

	unordered_map<CGameObject*, pair<D3DXVECTOR3, int>> playableObjectBackup;

	StaticObject* mapBackground;

	BigGate* gate;
	float countingTime1;
	float countingTime2;
	bool shiftingCamera = false;
	float deltaShiftY = 0;
	float deltaShiftX = 0;
	float totalShifting = 0;

	int state = State::_PLAYSCENE_FREE_PLAYING_;

	bool canSpawnPlayer = true;

	// switch scene
	int id_target_scene;
	float maxFading = 0.4;
	float totalFaded = 0;

protected:
	virtual void _ParseSection_TEXTURES(string line);
	virtual void _ParseSection_SPRITES(string line);
	virtual void _ParseSection_ANIMATIONS(string line);
	virtual void _ParseSection_OBJECTS(string line);
	virtual void _ParseSection_MAP(string line);
	virtual void _ParseSection_MERGEDBRICK(string line);
	virtual void _ParseSection_SHAREDDATA(string line);
	virtual void InitGUI();
	void ReloadSceneObject();
	void HardReloadSceneObject();

	virtual void ApllyVelocityToGameObjs(float dt);

	int GetMapBlockID(float x, float y);
	std::vector<int> GetMapBlockID(CGameObject* object);
	vector<CGameObject*> UpdateOnScreenObjs();

	void AddForegroundTile(ForegroundTile* tile);
	std::vector<ForegroundTile*> GetOnScreenForeGroundTiles();

	void BackupPlayableObject();
	virtual void UpdateFreePlaying(float dt);
	virtual void UpdateSwitchSection(float dt);
	virtual void UpdateFaddingIn(float dt);
	virtual void UpdateFaddingOut(float dt);

public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();
	void AddGameObjectToScene(CGameObject* obj);
	bool GameObjectIsInScene(CGameObject* obj);
	void RemoveGameObjectFromScene(CGameObject* obj);
	vector<CGameObject*> GetOnScreenObjs();
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void HardUnload();

	void ReloadBackup();
	void HardReload();
	
	DynamicObject* GetPlayer() { return player; }
	void SetPlayer(DynamicObject* newPlayer) { player = newPlayer; }

	void SwitchSection(BigGate* gate);
	void SwitchScene(int id_target_scene);
};