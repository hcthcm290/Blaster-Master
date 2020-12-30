#include "PlayScene.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include "Debug.h"
#include "Textures.h"
#include "Orb.h"
#include "AGR_Orb.h"
#include "Floater.h"
#include "Jumper.h"
#include "Jason.h"
#include "Insect.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <vector>
#include "StaticObject.h"
#include "Jumper.h"
#include "CollisionSystem.h"
#include "ColliableBrick.h"
#include "Camera.h"
#include "Mine.h"
#include "Dome.h"
#include <execution>
#include <algorithm>
#include <unordered_map>
#include "Bullet_Jason.h"
#include "BigGate.h"
#include "Spike.h"
#include "Sophia.h"
#include "Skull.h"
#include "Worm.h"
#include "Skull_Bullet.h"
#include "PlayerItem.h"
#include "Lava.h"
#include "Ladder.h"
#include "CameraBoundaryLib.h"
#include "PInput.h"
#include "ForegroundTile.h"
#include "SmallGate.h"
#include "Eyeball_Spawner.h"
#include "HealthBarGUI.h"
#include "StaticGUI.h"
#include "Rock.h"
#include "SoundManager.h"
#include "Ship.h"
#include "IDSceneConstant.h"
#include "TheEye.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
}
CPlayScene::~CPlayScene() {
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP 7
#define SCENE_SECTION_MERGEDBRICK 8
#define SCENE_SECTION_DEFAULTCAMERA 9
#define SCENE_SECTION_SHAREDDATA 10

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 2048

#pragma region Parse Scene Info
void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case -1:
		// MAP BACKGROUND //
		obj = new StaticObject();
		obj->SetPosition(x, y);
		dynamic_cast<StaticObject*>(obj)->SetSpriteID(-1);
		mapBackground = dynamic_cast<StaticObject*>(obj);
		return;
	case 1:
		obj = new Jumper();
		break;
	case 3:
		obj = new Mine();
		break;
	case 4:
	{
		int gravity = atoi(tokens[3].c_str());
		int direction = atoi(tokens[4].c_str());
		obj = new Dome(gravity, direction);
		break;
	}
	case 9:
		obj = new Orb();
		break;
	case -999:
		obj = new Jason();
		break;
	case 98:
	{
		if (!canSpawnPlayer)	return;

		obj = new Sophia();
		player = dynamic_cast<DynamicObject*>(obj);

		canSpawnPlayer = false;

		break;
	}
	case 5:
		obj = new Worm();
		break;
	case 7:
		obj = new Skull();
		break;
	case 2:
		obj = new Floater();
		break;
	case 6:
		obj = new Insect();
		break;
	case 76:
	{
		FRECT cameraBoundingBox;
		cameraBoundingBox.left = atoi(tokens[2].c_str()) * 16 - 8;
		cameraBoundingBox.top = atoi(tokens[3].c_str()) * 16 - 8;
		cameraBoundingBox.right = atoi(tokens[4].c_str()) * 16 + 8;
		cameraBoundingBox.bottom = atoi(tokens[5].c_str()) * 16 + 8;

		CameraBoundaryLib::AddCameraBoundary(tokens[1], cameraBoundingBox);
		return;
	}
	case 77:
	{
		BigGate* bg = new BigGate();

		x = atof(tokens[1].c_str()) * 16;
		y = atof(tokens[2].c_str()) * 16;

		bg->shift_direction = D3DXVECTOR2(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));

		x += bg->shift_direction.x * 4;

		bg->shift_time1 = atof(tokens[5].c_str());
		bg->shift_time2 = atof(tokens[6].c_str());

		bg->pre_teleport_delta = D3DXVECTOR2(atof(tokens[7].c_str()), atof(tokens[8].c_str()));
		//bg->new_boundary_camera = FRECT(atof(tokens[9].c_str()), atof(tokens[10].c_str()), atof(tokens[11].c_str()), atof(tokens[12].c_str()));

		bg->new_boundary_camera = CameraBoundaryLib::getCameraBoundary(tokens[9]);

		bg->teleport_delta = D3DXVECTOR2(atof(tokens[10].c_str()), atof(tokens[11].c_str()));

		//bg->shift_speed = atoi(tokens[15].c_str());


		obj = bg;
		break;
	}
	case 78:
	{
		obj = new SmallGate();

		dynamic_cast<SmallGate*>(obj)->SetIDTargetScene(atoi(tokens[3].c_str()));

		break;
	}
	case 0:
		obj = new PlayerItem();
		break;
	case 901:
		obj = new AGR_Orb();
		break;
	case 23:
	{
		int length = atoi(tokens[3].c_str());
		obj = new Spike(length);
		break;
	}
	case 30: {
		int length = atoi(tokens[3].c_str());
		obj = new Lava(length);
		break;
	}
	case 31: {
		int height = atoi(tokens[3].c_str());
		obj = new Ladder(height);
		break;
	}
	case 27: {
		obj = new Ship();
		break;
	}
	}

	// General object setup
	obj->SetPosition(x, y);

	AddGameObjectToScene(obj);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	string map_file_path = tokens[0];

	FILE* fp;
	errno_t err = fopen_s(&fp, map_file_path.c_str(), "r");

	char* readBuffer;
	readBuffer = new char[65536];

	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	std::vector<std::pair<std::string, int**>> mapInfo;

	auto layers = d["layers"].GetArray();
	auto texturePath = ToWSTR(d["tilesets"].GetArray()[0]["image"].GetString());

	int imgwidth = d["tilesets"].GetArray()[0]["imagewidth"].GetInt();
	int imgheight = d["tilesets"].GetArray()[0]["imageheight"].GetInt();

	int tilewidth = d["tilewidth"].GetInt();
	int tileheight = d["tileheight"].GetInt();

	int tilesetwidth = imgwidth / tilewidth;
	int tilesetheight = imgheight / tileheight;

	CTextures::GetInstance()->Add(0, texturePath.c_str(), D3DCOLOR_XRGB(255, 0, 255));

	for (auto& layer : d["layers"].GetArray())
	{
		string layerName = layer["name"].GetString();

		// we only care for the foreground for now
		if (layerName != "Foreground")	continue;

		const auto& data = layer["data"].GetArray();

		int** arr;

		int mapwidth = layer["width"].GetInt();
		int mapheight = layer["height"].GetInt();

		arr = new int* [mapwidth];

		for (int i = 0; i < mapwidth; i++)
		{
			arr[i] = new int[mapheight];
		}

		for (int x = 0; x < mapwidth; x++)
		{
			for (int y = 0; y < mapheight; y++)
			{
				int tileid = data[mapwidth * y + x].GetInt() - 1;

				if (tileid == -1)	continue;

				arr[x][y] = tileid;

				int x_index = tileid % tilesetwidth;
				int y_index = tileid / tilesetwidth;

				CSprites::GetInstance()->Add(tileid, x_index * tilewidth, y_index * tileheight, (x_index + 1) * tilewidth, (y_index + 1) * tileheight, CTextures::GetInstance()->Get(0));

				ForegroundTile* tile = new ForegroundTile();
				tile->SetPosition(x * tilewidth, y * tileheight);
				tile->SetSpriteID(tileid);

				AddForegroundTile(tile);
			}
		}

		mapInfo.emplace_back(layerName, arr);
	}

	delete [] readBuffer;

	fclose(fp);
}

void CPlayScene::_ParseSection_MERGEDBRICK(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	string map_file_path = tokens[0];

	FILE* fp;
	errno_t err = fopen_s(&fp, map_file_path.c_str(), "r"); // non-Windows use "r"

	char* readBuffer;
	readBuffer = new char[65536];

	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	auto listObject = d.GetArray();

	for (auto& object : listObject)
	{
		ColliableBrick* brick = new ColliableBrick();
		
		FRECT brickRECT = FRECT(
			object["left"].GetDouble(),
			object["top"].GetDouble(),
			object["right"].GetDouble(),
			object["bottom"].GetDouble()
		);

		brick->SetPosition((brickRECT.right + brickRECT.left) / 2 * 16, (brickRECT.bottom + brickRECT.top) / 2 * 16);
		brick->SetWidth((brickRECT.right - brickRECT.left + 1) * 16);
		brick->SetHeight((brickRECT.bottom - brickRECT.top + 1) * 16);

		AddGameObjectToScene(brick);
	}
}

void CPlayScene::_ParseSection_SHAREDDATA(string line)
{
	LPCWSTR backupFilePath = this->sceneFilePath;
	this->sceneFilePath = ToLPCWSTR(line);

	Load();

	this->sceneFilePath = backupFilePath;
}

void CPlayScene::InitGUI()
{
	HealthBarGUI* hpGUI = new HealthBarGUI();
	GUIObjects.emplace_back(hpGUI);

	StaticGUI* hovText = new StaticGUI(16100, D3DXVECTOR2(-108, 1));
	GUIObjects.emplace_back(hovText);
}

void CPlayScene::ReloadSceneObject()
{
	unordered_map<CGameObject*, int> listObject;

	for (auto& block : sceneObjects)
	{
		for (auto obj : block.second)
		{
			listObject[obj] = 1;
		}
		block.second.clear();
	}
	sceneObjects.clear();

	for (auto& obj : listObject)
	{
		delete obj.first;
	}

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;

		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MERGEDBRICK]") {
			section = SCENE_SECTION_MERGEDBRICK; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MERGEDBRICK: _ParseSection_MERGEDBRICK(line); break;
		}
	}

	f.close();
}

void CPlayScene::HardReloadSceneObject()
{
	Unload();

	Load();
}



#pragma endregion

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[MERGEDBRICK]") {
			section = SCENE_SECTION_MERGEDBRICK; continue;
		}
		if (line == "[DEFAULTCAMERA]")	{
			section = SCENE_SECTION_DEFAULTCAMERA; continue;
		}
		if (line == "[SHAREDDATA]") {
			section = SCENE_SECTION_SHAREDDATA; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_MERGEDBRICK: _ParseSection_MERGEDBRICK(line); break;
		case SCENE_SECTION_SHAREDDATA: _ParseSection_SHAREDDATA(line); break;
		case SCENE_SECTION_DEFAULTCAMERA: 
		{
			// we do not have to use the old way anymore
			// if(line != "")	Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::getCameraBoundary(line));
		}
		}
	}

	f.close();

	BackupPlayableObject();

	totalFaded = 0;
	state = State::_PLAYSCENE_FADDING_IN;

	Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));

	InitGUI();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::AddGameObjectToScene(CGameObject* obj)
{
	std::vector<int> listMapBlockID = GetMapBlockID(obj);

	for (auto mapBlockID : listMapBlockID)
	{
		if (dynamic_cast<Playable*>(obj) != NULL)
		{
			playableObjects[mapBlockID].emplace_back(obj);
		}
		else
		{
			sceneObjects[mapBlockID].emplace_back(obj);
		}
	}
}

bool CPlayScene::GameObjectIsInScene(CGameObject* obj)
{
	std::vector<int> listMapBlockID = GetMapBlockID(obj);

	for (auto mapBlockID : listMapBlockID)
	{
		auto e = std::find(sceneObjects[mapBlockID].begin(), sceneObjects[mapBlockID].end(), obj);

		return (e != sceneObjects[mapBlockID].end());
	}
}

void CPlayScene::RemoveGameObjectFromScene(CGameObject* obj)
{
	std::vector<int> listMapBlockID = GetMapBlockID(obj);

	for (auto mapBlockID : listMapBlockID)
	{
		if (dynamic_cast<Playable*>(obj) != NULL)
		{
			auto e = std::find(playableObjects[mapBlockID].begin(), playableObjects[mapBlockID].end(), obj);

			if (e != playableObjects[mapBlockID].end())
			{
				playableObjects[mapBlockID].erase(e);
			}
		}
		else
		{
			auto e = std::find(sceneObjects[mapBlockID].begin(), sceneObjects[mapBlockID].end(), obj);

			if (e != sceneObjects[mapBlockID].end())
			{
				sceneObjects[mapBlockID].erase(e);
			}
		}
	}
}

int CPlayScene::GetMapBlockID(float x, float y)
{
	return int(x / MAP_BLOCK_WIDTH) * 1000 + int(y / MAP_BLOCK_HEIGHT);
}

std::vector<int> CPlayScene::GetMapBlockID(CGameObject* object)
{
	std::vector<int> listMapBlockID;

	FRECT objectRECT = object->GetCollisionBox();

	FRECT objectInMapChunk = FRECT(objectRECT.left / MAP_BLOCK_WIDTH,
		objectRECT.top / MAP_BLOCK_HEIGHT,
		objectRECT.right / MAP_BLOCK_WIDTH,
		objectRECT.bottom / MAP_BLOCK_HEIGHT);

	for (int x = objectInMapChunk.left; x <= objectInMapChunk.right; x++)
	{
		for (int y = objectInMapChunk.top; y <= objectInMapChunk.bottom; y++)
		{
			listMapBlockID.emplace_back(x * 1000 + y);
		}
	}

	return listMapBlockID;
}

/// <summary>
/// Get all gameobjects appear on camera view
/// </summary>
/// <param name="target"></param>
vector<CGameObject*> CPlayScene::GetOnScreenObjs()
{
	return onScreenObjs;
}

vector<CGameObject*> CPlayScene::UpdateOnScreenObjs()
{
	onScreenObjs.clear();

	FRECT cameraRECT = Camera::GetInstance()->GetCollisionBox();

	std::unordered_map<CGameObject*, CGameObject*> listOnScreenObjs;
	vector<CGameObject*> arrPlayerObjs;

	FRECT cameraInMapChunk = FRECT(cameraRECT.left / MAP_BLOCK_WIDTH,
		cameraRECT.top / MAP_BLOCK_HEIGHT,
		cameraRECT.right / MAP_BLOCK_WIDTH,
		cameraRECT.bottom / MAP_BLOCK_HEIGHT);

	for (auto mapBlockID : GetMapBlockID(Camera::GetInstance()))
	{
		for (auto object : sceneObjects[mapBlockID])
		{
			if (dynamic_cast<ColliableBrick*>(object))
			{
				if (listOnScreenObjs.find(object) == listOnScreenObjs.end())
				{
					listOnScreenObjs[object] = object;
				}
			}
			else if (CollisionSystem::CheckOverlap(object, Camera::GetInstance()))
			{
				if (listOnScreenObjs.find(object) == listOnScreenObjs.end())
				{
					listOnScreenObjs[object] = object;
				}
			}
		}
	}
	//int count = 0;

	for (auto mapBlockID : GetMapBlockID(Camera::GetInstance()))
	{
		for (auto object : playableObjects[mapBlockID])
		{
			if (CollisionSystem::CheckOverlap(object, Camera::GetInstance()))
			{
				if (listOnScreenObjs.find(object) == listOnScreenObjs.end())
				{
					listOnScreenObjs[object] = object;
				}
			}
		}
	}

	for (auto object : listOnScreenObjs)
	{
		if (dynamic_cast<Jason*>(object.first) != NULL || dynamic_cast<Sophia*>(object.first) != NULL) {
			arrPlayerObjs.emplace_back(object.first);
		}
		else onScreenObjs.emplace_back(object.first);
	}

	for (auto player : arrPlayerObjs)
	{
		onScreenObjs.emplace_back(player);
	}

	return onScreenObjs;
}

void CPlayScene::AddForegroundTile(ForegroundTile* tile)
{
	std::vector<int> listMapBlockID = GetMapBlockID(tile);

	for (auto mapBlockID : listMapBlockID)
	{
		foregroundTiles[mapBlockID].emplace_back(tile);
	}
}

std::vector<ForegroundTile*> CPlayScene::GetOnScreenForeGroundTiles()
{
	FRECT cameraRECT = Camera::GetInstance()->GetCollisionBox();

	std::vector<ForegroundTile*> return_listOnScreenTiles;

	std::unordered_map<ForegroundTile*, ForegroundTile*> listOnScreenTile;

	FRECT cameraInMapChunk = FRECT(cameraRECT.left / MAP_BLOCK_WIDTH,
		cameraRECT.top / MAP_BLOCK_HEIGHT,
		cameraRECT.right / MAP_BLOCK_WIDTH,
		cameraRECT.bottom / MAP_BLOCK_HEIGHT);

	for (auto mapBlockID : GetMapBlockID(Camera::GetInstance()))
	{
		for (auto tile : foregroundTiles[mapBlockID])
		{
			if (CollisionSystem::CheckOverlap(tile, Camera::GetInstance()))
			{
				if (listOnScreenTile.find(tile) == listOnScreenTile.end())
				{
					listOnScreenTile[tile] = tile;
				}
			}
		}
	}

	for (auto tile : listOnScreenTile)
	{
		return_listOnScreenTiles.emplace_back(tile.first);
	}

	return return_listOnScreenTiles;
}

void CPlayScene::BackupPlayableObject()
{
	playableObjectBackup.clear();

	for (auto listObject : playableObjects)
	{
		for (auto object : listObject.second)
		{
			playableObjectBackup[object] = std::pair<D3DXVECTOR3, int>(object->GetPosition(), object->GetState());
		}
	}

	playerBackup = player;
}

void CPlayScene::UpdateFreePlaying(float dt)
{
	if (dt > 0.1) dt = 0.1;

	if (dt == 0) return;

	PInput::Update();

	if (DInput::KeyDown(DIK_RETURN) &&
		(dynamic_cast<Sophia*>(player) != NULL ||
		dynamic_cast<Jason*>(player) != NULL))
	{
		CGame::GetInstance()->SoftSwitchScene(IDSceneConstant::WEAPON_SELECTOR_SCENE, false, true);
		return;
	}

	if (dynamic_cast<Playable*>(player)->IsDead())
	{
		if (TheEye::GetInstance()->GetLifeLeft() > 0)
		{
			this->ReloadBackup();
			TheEye::GetInstance()->SetLifeLeft(TheEye::GetInstance()->GetLifeLeft() - 1);
			CGame::GetInstance()->SoftSwitchScene(IDSceneConstant::LIFE_LEFT_SCENE, false, true);
			return;
		}
		else
		{
			canSpawnPlayer = true;

			TheEye::GetInstance()->ResetLifeLeft();
			CGame::GetInstance()->SwitchScene(IDSceneConstant::GAME_OVER_SCENE);
			return;
		}
	}

	// Update for all the game object
	for (int i = 0; i< onScreenObjs.size(); i++)
	{
		onScreenObjs[i]->Update(dt);
	}

	for (int i = 0; i < onScreenObjs.size(); i++)
	{
		if (dynamic_cast<DynamicObject*>(onScreenObjs.at(i)) == NULL)
		{
			continue; // if it not moving, we don't need to docollision for it
		}
		else if (D3DXVECTOR3(dynamic_cast<DynamicObject*>(onScreenObjs.at(i))->GetVelocity()) == D3DXVECTOR3(0, 0, 0))
		{
			continue;
		}
		CollisionSystem::DoCollision(dynamic_cast<DynamicObject*>(onScreenObjs.at(i)), &onScreenObjs, dt);
	}

	ApllyVelocityToGameObjs(dt);

	Camera::GetInstance()->Update(dt);

	for (int i = 0; i < GUIObjects.size(); i++)
	{
		GUIObjects[i]->Update(dt);
	}
}

void CPlayScene::UpdateSwitchSection(float dt)
{
	#pragma region Update Enemy Object

	if (dt > 0.1) dt = 0.1;

	if (dt == 0) return;

	PInput::Update();

	// Update for all the game object
	for (int i = 0; i< onScreenObjs.size(); i++)
	{
		if (dynamic_cast<Playable*>(onScreenObjs[i]) != NULL)
			continue;
		onScreenObjs[i]->Update(dt);
	}

	for (int i = 0; i < onScreenObjs.size(); i++)
	{
		if (dynamic_cast<Playable*>(onScreenObjs[i]) != NULL)
			continue;

		if (dynamic_cast<DynamicObject*>(onScreenObjs.at(i)) == NULL)
		{
			continue; // if it not moving, we don't need to docollision for it
		}
		else if (D3DXVECTOR3(dynamic_cast<DynamicObject*>(onScreenObjs.at(i))->GetVelocity()) == D3DXVECTOR3(0, 0, 0))
		{
			continue;
		}
		CollisionSystem::DoCollision(dynamic_cast<DynamicObject*>(onScreenObjs.at(i)), &onScreenObjs, dt);
	}

	ApllyVelocityToGameObjs(dt);

	#pragma endregion

	countingTime1 += dt;

	RemoveGameObjectFromScene(player);

	float baseMovingSpeed = dynamic_cast<Playable*>(player)->GetEnterGateSpeed();

	if (countingTime1 < gate->shift_time1)
	{
		// shifting time 1
		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, player->GetPosition().y);
	}

	if (countingTime1 > gate->shift_time1 && countingTime2 == 0)
	{
		// shift camera
		shiftingCamera = true;

		if (deltaShiftX == 0) // that's mean we've not set it yet
		{
			if (gate->shift_direction.x > 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().right - Camera::GetInstance()->GetCollisionBox().left;
			}

			if (gate->shift_direction.x < 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().left - Camera::GetInstance()->GetCollisionBox().right;
			}
		}

		// shift camera follow y-axis to respect the new camera boundary
		auto cameraRECT = Camera::GetInstance()->GetCollisionBox();
		auto cameraPosition = Camera::GetInstance()->GetPosition();

		if (cameraRECT.bottom + gate->teleport_delta.y > gate->new_boundary_camera.bottom)
		{
			deltaShiftY = -(cameraRECT.bottom + gate->teleport_delta.y - gate->new_boundary_camera.bottom);
		}
		else if (cameraRECT.top + gate->teleport_delta.y < gate->new_boundary_camera.top)
		{
			deltaShiftY = -(cameraRECT.top + gate->teleport_delta.y - gate->new_boundary_camera.top);
		}
		else
		{
			deltaShiftY = 0;
		}

		if (deltaShiftY != 0)
		// if we still need to shift y-axis
		{
			int directionY = deltaShiftY / abs(deltaShiftY);

			if (abs(directionY * 150 * dt) < abs(deltaShiftY))
			{
				Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y + directionY * 150 * dt);
			}
			else
			{
				Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y + deltaShiftY);
			}
		}
		else
		// when we dont need to shift y-axis anymore, shift x-axis
		{
			Camera::GetInstance()->SetPosition(cameraPosition.x + gate->shift_direction.x * 150 * dt, cameraPosition.y);

			totalShifting += gate->shift_direction.x * 150 * dt;

			if (abs(totalShifting) >= abs(deltaShiftX))
			{
				shiftingCamera = false;

				auto playerPosition = player->GetPosition();

				player->SetPosition(playerPosition.x + gate->pre_teleport_delta.x, playerPosition.y + gate->pre_teleport_delta.y);
			}
		}
	}

	if (countingTime1 > gate->shift_time1 && shiftingCamera == false)
	{
		// shifting time 2
		countingTime2 += dt;

		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, player->GetPosition().y);
	}

	if (countingTime2 >= gate->shift_time2)
	{

		auto playerPosition = player->GetPosition();
		auto cameraPosition = Camera::GetInstance()->GetPosition();

		player->SetPosition(playerPosition.x + gate->teleport_delta.x, playerPosition.y + gate->teleport_delta.y);
		player->SetVelocity(0, 50);

		Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));

		Camera::GetInstance()->SetPosition(cameraPosition.x + gate->teleport_delta.x, cameraPosition.y + gate->teleport_delta.y);

		state = State::_PLAYSCENE_FREE_PLAYING_;
	}

	AddGameObjectToScene(player);
	BackupPlayableObject();
}

void CPlayScene::UpdateFaddingIn(float dt)
{
	UpdateFreePlaying(dt);

	totalFaded += dt;
	if (totalFaded >= maxFading)
	{
		CGame::GetInstance()->ToggleOverrideColorOff();
		state = State::_PLAYSCENE_FREE_PLAYING_;
		return;
	}

	int color = 255 * (totalFaded / maxFading);

	CGame::GetInstance()->ToggleOverrideColorOn();
	CGame::GetInstance()->SetOverrideColor(D3DCOLOR_ARGB(255, color, color, color));
}

void CPlayScene::UpdateFaddingOut(float dt)
{
	totalFaded += dt;
	if (totalFaded >= maxFading)
	{
		CGame::GetInstance()->ToggleOverrideColorOff();
		CGame::GetInstance()->SwitchScene(id_target_scene);
		return;
	}

	int color = 255 * (1 - totalFaded / maxFading);

	if (color < 70) color = 0;

	CGame::GetInstance()->ToggleOverrideColorOn();
	CGame::GetInstance()->SetOverrideColor(D3DCOLOR_ARGB(255, color, color, color));
}

void CPlayScene::Update(DWORD dw_dt)
{
	CollisionSystem::ClearPairColObjCache();

	UpdateOnScreenObjs();

	float dt = (float)(dw_dt);
	dt /= 1000;

	if (dt > 0.1)
		dt = 0.1;

	if (state == State::_PLAYSCENE_FREE_PLAYING_)
	{
		UpdateFreePlaying(dt);
	}
	else if (state == State::_PLAYSCENE_SWITCH_SECTION)
	{
		UpdateSwitchSection(dt);
	}
	else if (state == State::_PLAYSCENE_FADDING_IN)
	{
		UpdateFaddingIn(dt);
	}
	else if (state == State::_PLAYSCENE_FADDING_OUT)
	{
		UpdateFaddingOut(dt);
	}
}

void CPlayScene::ApllyVelocityToGameObjs(float dt)
{
	for (auto obj : onScreenObjs)
	{
		if (dynamic_cast<Playable*>(obj) != NULL && state == State::_PLAYSCENE_SWITCH_SECTION)
			continue;

		std::vector<int> oldListMapBlockID = GetMapBlockID(obj);

		if (dynamic_cast<DynamicObject*>(obj) == NULL) continue;
		D3DXVECTOR3 position = obj->GetPosition();
		D3DXVECTOR3 velocity = dynamic_cast<DynamicObject*>(obj)->GetVelocity();

		position = position + dt*velocity;

		obj->SetPosition(position.x, position.y);

		std::vector<int> newListMapBlockID = GetMapBlockID(obj);


		for (auto oldMapBlockID : oldListMapBlockID)
		{
			auto e = std::find(newListMapBlockID.begin(), newListMapBlockID.end(), oldMapBlockID);

			// if the old id dont appear in the new list id, delete the object from that map block
			if (e == newListMapBlockID.end())
			{
				if (dynamic_cast<Playable*>(obj) != NULL)
				{
					auto location = std::find(playableObjects[oldMapBlockID].begin(), playableObjects[oldMapBlockID].end(), obj);

					if (location != playableObjects[oldMapBlockID].end())
					{
						playableObjects[oldMapBlockID].erase(location);
					}
				}
				else
				{
					auto location = std::find(sceneObjects[oldMapBlockID].begin(), sceneObjects[oldMapBlockID].end(), obj);

					if (location != sceneObjects[oldMapBlockID].end())
					{
						sceneObjects[oldMapBlockID].erase(location);
					}
				}
			}
		}

		for (auto newMapBlockID : newListMapBlockID)
		{
			auto e = std::find(oldListMapBlockID.begin(), oldListMapBlockID.end(), newMapBlockID);
			// if the new id dont appear in the old list id, add the object to that map block
			if (e == oldListMapBlockID.end())
			{
				if (dynamic_cast<Playable*>(obj) != NULL)
				{
					playableObjects[newMapBlockID].emplace_back(obj);
				}
				else
				{
					sceneObjects[newMapBlockID].emplace_back(obj);
				}
			}
		}

		/*if (oldMapBlockID != newMapBlockID)
		{
			RemoveGameObjectFromScene(obj);		
			obj->SetPosition(position.x, position.y);
			AddGameObjectToScene(obj);
		}
		else
		{
			obj->SetPosition(position.x, position.y);
		}*/
	}
}

void CPlayScene::Render()
{
	if (!DInput::KeyPress(DIK_B))
	{
		mapBackground->Render();
	}

	for (int i = 0; i < onScreenObjs.size(); i++)
	{
		if (dynamic_cast<Rock*>(onScreenObjs[i]) != NULL) { onScreenObjs[i]->Render(); }
		if (dynamic_cast<ColliableBrick*>(onScreenObjs[i]) != NULL) continue;
		onScreenObjs[i]->Render();
	}

	auto foregroundTiles = GetOnScreenForeGroundTiles();
	for (auto tile : foregroundTiles)
	{
		tile->Render();
	}

	for (int i = 0; i < GUIObjects.size(); i++)
	{
		GUIObjects[i]->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	unordered_map<CGameObject*, int> listObject;

	for (auto& block : sceneObjects)
	{
		for (auto obj : block.second)
		{
			listObject[obj] = 1;
		}
		block.second.clear();
	}
	sceneObjects.clear();

	for (auto& obj : listObject)
	{
		delete obj.first;
	}

	unordered_map<DynamicObject*, int> listPlayable;
	for (auto& block : playableObjects)
	{
		for (auto obj : block.second)
		{
			listPlayable[dynamic_cast<DynamicObject*>(obj)] = 1;
		}
	}

	for (auto& obj : listPlayable)
	{
		obj.first->SetAnimator(NULL);

		if (canSpawnPlayer) delete obj.first;
	}

	if (canSpawnPlayer) playableObjects.clear();

	onScreenObjs.clear();
}

void CPlayScene::HardUnload()
{
	canSpawnPlayer = true;
	Unload();
}

void CPlayScene::ReloadBackup()
{
	canSpawnPlayer = false;

	ReloadSceneObject();

	playableObjects.clear();
	onScreenObjs.clear();

	for (auto backup : playableObjectBackup)
	{
		auto backupObj = backup.first;
		backupObj->SetPosition(backup.second.first.x, backup.second.first.y);
		backupObj->SetState(backup.second.second);
		auto d_backupObj = dynamic_cast<DynamicObject*>(backupObj);
		d_backupObj->SetCurrentHP(d_backupObj->GetMaxHP());

		AddGameObjectToScene(backupObj);

		// This part change the Jason in TheEye
		// This problem is cause because of we create a new Jason everytime Jason jump out of Sophia and we not delete the old Jason*
		// So, the Jason* when save backup may not be the same as the Jason* at TheEye 
		// We may fix this in later patch
		if (dynamic_cast<Jason*>(backupObj) != NULL)
		{
			TheEye::GetInstance()->SetJason(dynamic_cast<Jason*>(backupObj));
		}
	}

	player = playerBackup;

	Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));
}

void CPlayScene::HardReload()
{
	canSpawnPlayer = true;

	HardReloadSceneObject();

}

void CPlayScene::SwitchSection(BigGate* gate)
{
	this->gate = gate;
	this->state = State::_PLAYSCENE_SWITCH_SECTION;
	countingTime1 = 0;
	countingTime2 = 0;
	shiftingCamera = false;
	deltaShiftX = 0;
	deltaShiftY = 0;
	totalShifting = 0;
}

void CPlayScene::SwitchScene(int id_target_scene)
{
	this->id_target_scene = id_target_scene;
	this->totalFaded = 0;
	state = State::_PLAYSCENE_FADDING_OUT;
	PInput::ClearBuffer();
}
