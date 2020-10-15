#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

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
	BYTE  keyStates[256];

public:
	static DInput* GetInstance();
	static bool KeyDown(int keyCode);
};