#include "DInput.h"
#include "Debug.h"

DInput* DInput::__instance = NULL;

void DInput::Init(HWND hWnd)
{
	HRESULT hr = DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	if (FAILED(hr))
	{
		DebugOut(L"Initiallize Direct Input fail");
		return;
	}
	
	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	if (FAILED(hr))
	{
		DebugOut(L"Initiallize Keyboard Device fail");
		return;
	}

	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	if (FAILED(hr))
	{
		DebugOut(L"Initiallize Mouse Device fail");
		return;
	}

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hWnd, DISCL_NOWINKEY | DISCL_FOREGROUND);

}

void DInput::ProcessKeyboard()
{
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

	for (int i = 0; i < 255; i++)
	{
		oldkeyStates[i] = keyStates[i];
	}

	DIKeyboard->GetDeviceState(sizeof(keyStates), (LPVOID)&keyStates);
}

void DInput::Release()
{
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
}

DInput* DInput::GetInstance()
{
	if (__instance == NULL)
	{
		__instance = new DInput();
	}

	return __instance;
}

/// <summary>
/// return true if key is move from unpress to press
/// </summary>
/// <param name="keyCode"></param>
/// <returns></returns>
bool DInput::KeyDown(int keyCode)
{
	return (__instance->keyStates[keyCode] && 0x80) && !(__instance->oldkeyStates[keyCode] && 0x80);
}

/// <summary>
/// return true if key is move from press to unpress
/// </summary>
/// <param name="keyCode"></param>
/// <returns></returns>
bool DInput::KeyUp(int keyCode)
{
	return !(__instance->keyStates[keyCode] && 0x80) && (__instance->oldkeyStates[keyCode] && 0x80);
}

/// <summary>
/// return true if key is press
/// </summary>
/// <param name="keyCode"></param>
/// <returns></returns>
bool DInput::KeyPress(int keyCode)
{
	return __instance->keyStates[keyCode] && 0x80;
}

bool DInput::MouseDown(int keyCode)
{
	return __instance->mouseState.rgbButtons[keyCode] && 0x80;
}

D3DXVECTOR2 DInput::GetMousePosition()
{
	return D3DXVECTOR2{ FLOAT(__instance->mouseState.lX) , FLOAT(__instance->mouseState.lY) };
}
