#include "Sound.h"
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
	
	#pragma region SoundLoad
	DebugOut(L"[INFO] Start loading sound resources from : /Sound \n");
	Sound::getInstance()->loadSound((char*)"Sound/3Missile.wav", "3Missile");
	Sound::getInstance()->loadSound((char*)"Sound/BigObjectJump.wav", "BigObjectJump");
	Sound::getInstance()->loadSound((char*)"Sound/BossEnter.wav", "BossEnter");
	Sound::getInstance()->loadSound((char*)"Sound/BossDie.wav", "BossDie");
	Sound::getInstance()->loadSound((char*)"Sound/DomeFLying.wav", "DomeFLying");
	Sound::getInstance()->loadSound((char*)"Sound/DomeWalking.wav", "DomeWalking");
	Sound::getInstance()->loadSound((char*)"Sound/DontKnow.wav", "DontKnow");
	Sound::getInstance()->loadSound((char*)"Sound/EnemyDie.wav", "EnemyDie");
	Sound::getInstance()->loadSound((char*)"Sound/EnemyOnHit.wav", "EnemyOnHit");
	Sound::getInstance()->loadSound((char*)"Sound/Explosive_bullet.wav", "Explosive_bullet");
	Sound::getInstance()->loadSound((char*)"Sound/EyeballSHOT.wav", "EyeballSHOT");
	Sound::getInstance()->loadSound((char*)"Sound/FullHeath.wav", "FullHeath");
	Sound::getInstance()->loadSound((char*)"Sound/Grenade.wav", "Grenade");
	Sound::getInstance()->loadSound((char*)"Sound/HomingMissile.wav", "HomingMissile");
	Sound::getInstance()->loadSound((char*)"Sound/Insect.wav", "Insect");
	Sound::getInstance()->loadSound((char*)"Sound/JasonBullet.wav", "JasonBullet");
	Sound::getInstance()->loadSound((char*)"Sound/JasonDie.wav", "JasonDie");
	Sound::getInstance()->loadSound((char*)"Sound/JasonGotHit_Interior.wav", "JasonGotHit_Interior");
	Sound::getInstance()->loadSound((char*)"Sound/JasonGotHit_Outside.wav", "JasonGotHit_Outside");
	Sound::getInstance()->loadSound((char*)"Sound/JasonJump.wav", "JasonJump");
	Sound::getInstance()->loadSound((char*)"Sound/JumperJump.wav", "JumperJump");
	Sound::getInstance()->loadSound((char*)"Sound/MineExplosion.wav", "MineExplosion");
	Sound::getInstance()->loadSound((char*)"Sound/MineWaiting.wav", "MineWaiting");
	Sound::getInstance()->loadSound((char*)"Sound/PowerUp.wav", "PowerUp");
	Sound::getInstance()->loadSound((char*)"Sound/SkullBullet.wav", "SkullBullet");
	Sound::getInstance()->loadSound((char*)"Sound/SophiaBullet.wav", "SophiaBullet");
	Sound::getInstance()->loadSound((char*)"Sound/swapSophiaAndJason.wav", "swapSophiaAndJason");
	Sound::getInstance()->loadSound((char*)"Sound/SophiaGoingSmall.wav", "SophiaGoingSmall");
	Sound::getInstance()->loadSound((char*)"Sound/SophiaDie.wav", "SophiaDie");
	Sound::getInstance()->loadSound((char*)"Sound/InteriorTheme.wav", "InteriorTheme");
	Sound::getInstance()->loadSound((char*)"Sound/EndingTheme.wav", "EndingTheme");
	Sound::getInstance()->loadSound((char*)"Sound/WormMoving_CannonShot_TeleporterShot.wav", "WormMoving_CannonShot_TeleporterShot");
	Sound::getInstance()->loadSound((char*)"Sound/TeleporterMoving.wav", "TeleporterMoving");
	Sound::getInstance()->loadSound((char*)"Sound/enter.wav", "enter");
	Sound::getInstance()->loadSound((char*)"Sound/intro.wav", "intro");
	Sound::getInstance()->loadSound((char*)"Sound/Thunder.wav", "Thunder");
	Sound::getInstance()->loadSound((char*)"Sound/DontKnow.wav", "DontKnow");
	Sound::getInstance()->loadSound((char*)"Sound/ThemeSong.wav", "ThemeSong");
#pragma endregion

}


void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, bool flipX, float angle, D3DCOLOR Color, float z_index)
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
	D3DXMatrixTranslation(&translate, (int)(x), (int)(y), z_index);

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

void CGame::FlushAllSceneData()
{
	for (auto scene : this->scenes)
	{
		if (dynamic_cast<CPlayScene*>(scene.second))
		{
			dynamic_cast<CPlayScene*>(scene.second)->HardUnload();
		}
		else
		{
			scene.second->Unload();
		}
	}
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	//SoundManager::GetInstance()->Release();
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
void SoundManager(int scene)
{
	switch (scene)
	{
	case (IDSceneConstant::TITLE_SCENE):
		Sound::getInstance()->play("intro", true, 0);
		break;
	case (IDSceneConstant::CAR_SCENE):
		Sound::getInstance()->stop("intro");
		Sound::getInstance()->play("enter", true, 0);
		break;
	case (IDSceneConstant::AREA2_SCENE):
		Sound::getInstance()->stop("enter");
		Sound::getInstance()->play("ThemeSong", true, 0);
		Sound::getInstance()->setVolume(95, "ThemeSong");
		break;
	case (IDSceneConstant::WEAPON_SELECTOR_SCENE):
		//Sound::getInstance()->setVolume(0, "ThemeSong");
		Sound::getInstance()->play("MineWaiting", false, 1);
		break;
	case (IDSceneConstant::LIFE_LEFT_SCENE):
		Sound::getInstance()->stop("ThemeSong");
		break;
	case IDSceneConstant::GAME_OVER_SCENE:
		Sound::getInstance()->stop("ThemeSong");
		break;
	case IDSceneConstant::OUTRO_SCENE:
		Sound::getInstance()->stop("ThemeSong");
		Sound::getInstance()->play("EndingTheme",false,1);
		break;
	default:
		break;
	}

	std::vector<int> listInteriorSceneID;
	listInteriorSceneID.emplace_back(2);
	listInteriorSceneID.emplace_back(3);
	listInteriorSceneID.emplace_back(4);
	listInteriorSceneID.emplace_back(5);

	// check for the interior scene
	if (std::find(listInteriorSceneID.begin(), listInteriorSceneID.end(), scene) != listInteriorSceneID.end())
	{
		Sound::getInstance()->play("ThemeSong", true, 1);
	}
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
	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	SoundManager(scene_id);
	s->Load();
}

void CGame::SoftSwitchScene(int scene_id, bool unloadPrevScene, bool loadNextScene)
{
	if (scene_stack.Contain(scene_id))
	{
		scene_stack.PopToValue(scene_id);
	}
	scene_stack.Push(scene_id);

	if (unloadPrevScene)
	{
		scenes[current_scene]->Unload();
	}

	current_scene = scene_id;

	if (loadNextScene)
	{
		scenes[current_scene]->Load();
	}
	SoundManager(scene_id);
}
HWND CGame::GetCurrentHWND() 
{
	return this->hWnd;
}
