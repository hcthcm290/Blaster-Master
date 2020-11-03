#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <D3dx9math.h>

#define LEFT_MOUSE_KEYCODE DIMOFS_BUTTON0
#define RIGHT_MOUSE_KEYCODE DIMOFS_BUTTON1

class DInput 
{
private:
	DInput() {};
	static DInput* __instance;

	void Init(HWND hWnd);
	void ProcessKeyboard();
	void Release();

	friend class CGame;

private:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	LPDIRECTINPUT8 DirectInput;
	DIMOUSESTATE mouseState;
	BYTE keyStates[256];
	BYTE oldkeyStates[256];

public:
	static DInput* GetInstance();
	static bool KeyDown(int keyCode);
	static bool KeyUp(int keyCode);
	static bool KeyPress(int keyCode);
	static bool MouseDown(int keycode);
	static D3DXVECTOR2 GetMousePosition();
};