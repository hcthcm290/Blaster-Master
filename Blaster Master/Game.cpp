#include "Debug.h"
#include "Game.h"
#include "Utils.h"
#include "PlayScene.h"
#include "Textures.h"
#include "Camera.h"
#include "InteriorScene.h"
#include <fstream>
#include "CameraBoundaryLib.h"
#include "MovieScene.h"
#include "SoundManager.h"

CGame* CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom;
	d3dpp.BackBufferWidth = r.right;

	screenHeight = r.bottom;
	screenWidth = r.right;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");

	DInput::GetInstance()->Init(hWnd);
}


void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, bool flipX, float angle, D3DCOLOR Color)
{
	if (this->beingOverrideColor)
		Color = overrideColor;

	D3DXVECTOR3 p(0, 0, 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXVECTOR3 center((right - left) / 2, (bottom - top) / 2, 0);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, (int)(x), (int)(y), 0);

	D3DXMATRIX toCameraView;
	D3DXMatrixTranslation(&toCameraView, -Camera::GetInstance()->GetCollisionBox().left, -Camera::GetInstance()->GetCollisionBox().top, 0);

	D3DXMATRIX flip;
	if (!flipX)
	{
		D3DXMatrixScaling(&flip, 1, 1, 0);
	}
	else
	{
		D3DXMatrixScaling(&flip, -1, 1, 0);
	}

	D3DXMATRIX rotate;
	D3DXMatrixRotationAxis(&rotate, new D3DXVECTOR3(0, 0, 1), angle*3.14/180);

	/*translate = translate * toCameraView;

	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			translate[i, j] = int(translate[i, j]);
		}*/

	mat *= flip;
	mat *= rotate;
	mat *= translate;
	mat *= toCameraView;

	spriteHandler->SetTransform(&mat);

	spriteHandler->Draw(texture, &r, &center, NULL, Color);
}

/*
	Utility function to load texture
*/
LPDIRECT3DTEXTURE9 CGame::LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	HRESULT result = D3DXGetImageInfoFromFile(texturePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", texturePath);
		return NULL;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,			// Transparent color
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed. File: %s\n", texturePath);
		return NULL;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
	return texture;
}


Stack<int> CGame::GetSceneStack()
{
	return Stack<int>(this->scene_stack);
}

LPSCENE CGame::GetScene(int id)
{
	return this->scenes[id];
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	SoundManager::GetInstance()->Release();
}


CGame* CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	int sceneType = atoi(tokens[2].c_str());
	LPSCENE scene = NULL;
	switch (sceneType)
	{
	case 0:
		scene = new BasicScene(id, path);
		break;
	case 1:
		scene = new CPlayScene(id, path);
		break;
	case 2:
		scene = new InteriorScene(id, path);
		break;
	case 3:
		scene = new MovieScene(id, path);
	}
	scenes[id] = scene;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2


void CGame::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}

void CGame::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	if (scene_stack.Contain(scene_id))
	{
		scene_stack.PopToValue(scene_id);
	}
	scene_stack.Push(scene_id);

	scenes[current_scene]->Unload();;

	CTextures::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	CameraBoundaryLib::ClearLib();
	SoundManager::GetInstance()->StopSource();
	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	if (scene_id == 1) //Main scene 
		SoundManager::GetInstance()->PlaySoundInfinite("ThemeSong.wav");
	else //interior 
		SoundManager::GetInstance()->PlaySoundInfinite("InteriorTheme.wav");
	s->Load();
}
