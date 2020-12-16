#include "BasicScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Debug.h"
#include <fstream>
#include "Camera.h"
#include "WeaponSelector.h"
#include "LifeShowner.h"

#define BASICSCENE_SECTION_UNKNOWN -1
#define BASICSCENE_SECTION_TEXTURES 1
#define BASICSCENE_SECTION_SPRITES 2
#define BASICSCENE_SECTION_ANIMATIONS 3
#define BASICSCENE_SECTION_OBJECTS 4
#define BASICSCENE_SECTION_BACKGROUNDSPRITEID 5
#define MAX_SCENE_LINE 2048

void BasicScene::_ParseSection_TEXTURES(std::string line)
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

void BasicScene::_ParseSection_SPRITES(std::string line)
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

void BasicScene::_ParseSection_ANIMATIONS(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

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

void BasicScene::_ParseSection_OBJECTS(std::string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());

	CGameObject* obj = NULL;

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	switch (object_type)
	{
	case 41:
	{
		obj = new WeaponSelector();
		dynamic_cast<StaticObject*>(obj)->SetSpriteID(atoi(tokens[3].c_str()));
		this->listSceneObject.emplace_back(obj);
		return;
	}
	case 42:
	{
		obj = new LifeShowner();
	}
	}

	if (obj != NULL)
	{
		this->listSceneObject.emplace_back(obj);
	}
}

BasicScene::BasicScene(int id, LPCWSTR filePath)
	:
	CScene(id, filePath)
{
}

void BasicScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = BASICSCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = BASICSCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = BASICSCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = BASICSCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[OBJECTS]") {
			section = BASICSCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[BACKGROUNDSPRITEID]") {
			section = BASICSCENE_SECTION_BACKGROUNDSPRITEID; continue;
		}
		if (line[0] == '[') { section = BASICSCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case BASICSCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case BASICSCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case BASICSCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case BASICSCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case BASICSCENE_SECTION_BACKGROUNDSPRITEID: {
			std::vector<string> tokens = split(line);
			if (tokens[0] == "") continue;

			if (atoi(tokens[0].c_str()) == BASICSCENE_INVALID_BACKGROUND_ID)
			{
				DebugOut(L"Invalid background id: %d, please try another id\n", atoi(tokens[0].c_str()));
			}

			this->BackgroundSpriteID = atoi(tokens[0].c_str());
		}
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void BasicScene::Unload()
{
	this->listSceneObject.clear();
}

void BasicScene::Update(DWORD dw_dt)
{
	float dt = (float)dw_dt / 1000;

	if (dt <= 0) return;

	for (int i = 0; i < listSceneObject.size(); i++)
	{
		listSceneObject[i]->Update(dt);
	}

	Camera::GetInstance()->SetPosition(0, 0);
}

void BasicScene::Render()
{
	if (this->BackgroundSpriteID != BASICSCENE_INVALID_BACKGROUND_ID)
	{
		CSprites::GetInstance()->Get(this->BackgroundSpriteID)->Draw(0, 0);
	}

	for (int i = 0; i < listSceneObject.size(); i++)
	{
		listSceneObject[i]->Render();
	}
}
