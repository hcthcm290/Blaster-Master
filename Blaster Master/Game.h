#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Scene.h"
#include "DInput.h"
#include "Stack.h"
using namespace std;

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	std::unordered_map<int, LPSCENE> scenes;
	int current_scene;
	Stack<int> scene_stack;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	int screenWidth;
	int screenHeight;

	D3DCOLOR overrideColor;
	bool beingOverrideColor = false;

public:
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, bool flipX = false, float angle = 0, D3DCOLOR Color = D3DCOLOR_XRGB(255,255,255), float z_index = 0.5);
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor);
	void ProcessKeyboard() { DInput::GetInstance()->ProcessKeyboard(); };

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);
	void SoftSwitchScene(int scene_id, bool unloadPrevScene, bool loadNextScene);

	static CGame* GetInstance();

	void ToggleOverrideColorOn() { this->beingOverrideColor = true; }
	void ToggleOverrideColorOff() { this->beingOverrideColor = false; }
	void SetOverrideColor(D3DCOLOR color) { this->overrideColor = color; }
	Stack<int> GetSceneStack();
	LPSCENE GetScene(int id);
	void FlushAllSceneData();

	~CGame();
};