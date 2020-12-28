#include "MovieScene.h"
#include <vector>
#include "Utils.h"
#include "Textures.h"
#include "Debug.h"
#include "Camera.h"
#include "Game.h"
#include <fstream>
#include "IDSceneConstant.h"
#include "PlayScene.h"

#define MOVIESCENE_SECTION_UNKNOWN -1
#define MOVIESCENE_SECTION_TEXTURES 2
#define MOVIESCENE_SECTION_SPRITES 3
#define MOVIESCENE_SECTION_ANIMATIONS 4
#define MOVIESCENE_SECTION_OBJECTS	5
#define MOVIESCENE_SECTION_TIMEOUT 6
#define MOVIESCENE_SECTION_MOVIEANIMATIONID 7
#define MOVIESCENE_SECTION_LISTBACKGROUNDCOLOR 9
#define MOVIESCENE_SECTION_BACKGROUNDCOLORPROPERTY 10
#define MOVIESCENE_SECTION_IDSCENE 11
#define MOVIESCENE_SECTION_ANIMATIONOFFSET 12

#define MAX_SCENE_LINE 2048

void MovieScene::_ParseSection_ListBackgroundColor(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() == 0 || tokens[0] == "") return;

	int r = atoi(tokens[0].c_str());
	int g = atoi(tokens[1].c_str());
	int b = atoi(tokens[2].c_str());
	float livingTime = atof(tokens[3].c_str()) / 1000; // convert milisecond to second
	listBackgroundColor.emplace_back(pair<D3DXCOLOR, float>(D3DCOLOR_ARGB(255, r, g, b), livingTime));
}

MovieScene::MovieScene(int id, LPCWSTR filePath)
	:
	BasicScene(id, filePath)
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
			section = MOVIESCENE_SECTION_MOVIEANIMATIONID; continue;
		}
		if (line == "[LISTBACKGROUNDCOLOR]") {
			section = MOVIESCENE_SECTION_LISTBACKGROUNDCOLOR; continue;
		}
		if (line == "[BACKGROUNDCOLORPROPERTY]") {
			section = MOVIESCENE_SECTION_BACKGROUNDCOLORPROPERTY; continue;
		}
		if (line == "[IDSCENE]") {
			section = MOVIESCENE_SECTION_IDSCENE; continue;
		}
		if (line == "[ANIMATIONOFFSET]") {
			section = MOVIESCENE_SECTION_ANIMATIONOFFSET; continue;
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

			if (tokens.size() == 0 || tokens[0] == "") continue;

			timeout = atof(tokens[0].c_str()) / 1000; // convert milisecond to second
			break;
		}
		case MOVIESCENE_SECTION_MOVIEANIMATIONID:{
			vector<string> tokens = split(line);

			if (tokens.size() == 0 || tokens[0] == "") continue;

			MovieAnimationID = atoi(tokens[0].c_str());

			this->movieAnimator = new Animator();
			this->movieAnimator->AddAnimation(MovieAnimationID);
			break;
		}
		case MOVIESCENE_SECTION_LISTBACKGROUNDCOLOR: {
			_ParseSection_ListBackgroundColor(line);
			break;
		}
		case MOVIESCENE_SECTION_BACKGROUNDCOLORPROPERTY: {
			vector<string> tokens = split(line);
			if (tokens.size() == 0 || tokens[0] == "") continue;

			BackgroundSpriteID = atoi(tokens[0].c_str());
			backgroundColorDelay = atof(tokens[1].c_str()) / 1000; // convert milisecond to second
			backgroundColorLivingTime = atof(tokens[2].c_str()) / 1000;

			int r = atoi(tokens[3].c_str());
			int g = atoi(tokens[4].c_str());
			int b = atoi(tokens[5].c_str());

			defaultBackgroundColor = D3DCOLOR_ARGB(255, r, g, b);
			break;
		}
		case MOVIESCENE_SECTION_IDSCENE: {
			vector<string> tokens = split(line);
			if (tokens.size() == 0 || tokens[0] == "") continue;

			idNextScene = atoi(tokens[0].c_str());

			if (tokens.size() == 2)
			{
				idBreakOutScene = atoi(tokens[1].c_str());
				canBreakOut = true;
			}
			break;
		}
		case MOVIESCENE_SECTION_ANIMATIONOFFSET: {
			vector<string> tokens = split(line);
			if (tokens.size() == 0 || tokens[0] == "") continue;

			float offset_x = atof(tokens[0].c_str());
			float offset_y = atof(tokens[1].c_str());

			animationOffset = D3DXVECTOR2(offset_x, offset_y);

			break;
		}
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	Camera::GetInstance()->SetPosition(0, 0);
}

void MovieScene::Unload()
{
	this->currentBackgroundColorIndex = -1;
	this->lastColorChoosenTime = -1;
	this->countTime = 0;
}

void MovieScene::Update(DWORD dw_dt)
{
	float dt = (float)dw_dt / 1000;

	if (dt > 0.1) dt = 0.1;

	countTime += dt;

	if (countTime >= timeout)
	{
		CGame::GetInstance()->SwitchScene(this->idNextScene);

		// before switching from movie into playscene, we must show the life left scene
		if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetScene(idNextScene)))
		{
			CGame::GetInstance()->SoftSwitchScene(IDSceneConstant::LIFE_LEFT_SCENE, false, true);
		}
		return;
	}

	if (DInput::KeyDown(DIK_RETURN) && canBreakOut)
	{
		CGame::GetInstance()->SwitchScene(this->idBreakOutScene);
		return;
	}

	if (countTime >= backgroundColorDelay + backgroundColorLivingTime)
	{
		currentBackgroundColorIndex = -1;
	}
	else if (lastColorChoosenTime == -1 && countTime >= backgroundColorDelay)
	{
		lastColorChoosenTime = countTime;
		currentBackgroundColorIndex = 0;
	}
	else if (countTime - lastColorChoosenTime > listBackgroundColor[currentBackgroundColorIndex].second)
	{
		lastColorChoosenTime = countTime;
		currentBackgroundColorIndex = (currentBackgroundColorIndex + 1) % listBackgroundColor.size();
	}

	for (auto object : this->listSceneObject)
	{
		object->Update(dt);
	}
}

void MovieScene::Render()
{
	if (BackgroundSpriteID != BASICSCENE_INVALID_BACKGROUND_ID)
	{
		if (currentBackgroundColorIndex != -1)
		{
			CSprites::GetInstance()->Get(BackgroundSpriteID)->Draw(0, 0, false, 0, listBackgroundColor[currentBackgroundColorIndex].first);
		}
		else
		{
			CSprites::GetInstance()->Get(BackgroundSpriteID)->Draw(0, 0, false, 0, defaultBackgroundColor);
		}
	}
	movieAnimator->Draw(this->MovieAnimationID, animationOffset.x, animationOffset.y, false);

	for (auto object : this->listSceneObject)
	{
		object->Render();
	}
}
