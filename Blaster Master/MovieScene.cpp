#include "MovieScene.h"
#include <vector>
#include "Utils.h"
#include "Textures.h"
#include "Debug.h"
#include <fstream>

#define MOVIESCENE_SECTION_UNKNOWN -1
#define MOVIESCENE_SECTION_TEXTURES 2
#define MOVIESCENE_SECTION_SPRITES 3
#define MOVIESCENE_SECTION_ANIMATIONS 4
#define MOVIESCENE_SECTION_OBJECTS	5
#define MOVIESCENE_SECTION_TIMEOUT 6
#define MOVIESCENE_SECTION_MOVIEANIMATIONID 7
#define MOVIESCENE_SECTION_BACKGROUNDSPRITEID 8

#define MAX_SCENE_LINE 2048

void MovieScene::_ParseSection_TEXTURES(string line)
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

void MovieScene::_ParseSection_SPRITES(string line)
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

void MovieScene::_ParseSection_ANIMATIONS(string line)
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

void MovieScene::_ParseSection_OBJECTS(string line)
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
	}
}

MovieScene::MovieScene(int id, LPCWSTR filePath)
	:
	CScene(id, filePath)
{
}

void MovieScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = MOVIESCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = MOVIESCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = MOVIESCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = MOVIESCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[OBJECTS]") {
			section = MOVIESCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TIMEOUT]") {
			section = MOVIESCENE_SECTION_TIMEOUT; continue;
		}
		if (line == "[ANIMATIONID]") {
			section = MOVIESCENE_SECTION_MOVIEANIMATIONID;
		}
		if (line == "[BACKGROUNDSPRITEID]"){
			section = MOVIESCENE_SECTION_BACKGROUNDSPRITEID;
		}
		if (line[0] == '[') { section = MOVIESCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case MOVIESCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case MOVIESCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case MOVIESCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case MOVIESCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case MOVIESCENE_SECTION_TIMEOUT: {
			vector<string> tokens = split(line);

			if (tokens.size() == 0) continue;

			timeout = atof(tokens[0].c_str());
			break;
		}
		case MOVIESCENE_SECTION_MOVIEANIMATIONID:{
			vector<string> tokens = split(line);

			if (tokens.size() == 0) continue;

			MovieAnimationID = atoi(tokens[0].c_str());
			break;
		}
		case MOVIESCENE_SECTION_BACKGROUNDSPRITEID:{
			vector<string> tokens = split(line);

			if (tokens.size() == 0) continue;

			BackgroundSpriteID = atoi(tokens[0].c_str());
			break;
		}
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void MovieScene::Unload()
{
}

void MovieScene::Update(DWORD dt)
{
}

void MovieScene::Render()
{
}
