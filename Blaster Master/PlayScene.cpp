#include "PlayScene.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include "Debug.h"
#include "Textures.h"
#include "Orb.h"
#include "Jason.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <vector>
#include "StaticObject.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
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

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


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

/*
	Parse a line in section [OBJECTS]
*/
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
	case 0:
		obj = new Orb();
		break;
	case 3:
		obj = new Jason();
		break;
	}

	// General object setup
	obj->SetPosition(x, y);

	objects.push_back(obj);
}

using namespace rapidjson;

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	string map_file_path = tokens[0];

	FILE* fp;
	errno_t err = fopen_s(&fp, map_file_path.c_str(), "r"); // non-Windows use "r"

	char* readBuffer;
	readBuffer = new char[65536];

	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
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

		const auto& data = layer["data"].GetArray();

		int** arr;

		int mapwidth = layer["width"].GetInt();
		int mapheight = layer["height"].GetInt();

		string layerName = layer["name"].GetString();

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
				arr[x][y] = tileid;

				int x_index = tileid % tilesetwidth;
				int y_index = tileid / tilesetwidth;

				if (CSprites::GetInstance()->Get(tileid) == NULL)
				{
					CSprites::GetInstance()->Add(tileid, x_index * tilewidth, y_index * tileheight, (x_index + 1) * tilewidth, (y_index + 1) * tileheight, CTextures::GetInstance()->Get(0));
				}

				CGameObject* tile = new StaticObject();
				tile->SetPosition(x * tilewidth, y * tileheight);
				dynamic_cast<StaticObject*>(tile)->SetSpriteID(tileid);
				objects.emplace_back(tile);
			}
		}

		mapInfo.emplace_back(layerName, arr);
	}

	fclose(fp);
}

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
		}
	}

	f.close();

	

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	for (auto& obj : objects) {
		obj->Update(dt);
	}
}

void CPlayScene::Render()
{
	for (auto& obj : objects) {
		obj->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (auto& obj : objects)
		delete obj;

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}