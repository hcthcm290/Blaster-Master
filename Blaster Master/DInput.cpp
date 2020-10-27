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

bool DInput::KeyDown(int keyCode)
{
	return __instance->keyStates[keyCode] && 0x80;
}

bool DInput::MouseDown(int keyCode)
{
	return __instance->mouseState.rgbButtons[keyCode] && 0x80;
}

D3DXVECTOR2 DInput::GetMousePosition()
{
	return D3DXVECTOR2{ __instance->mouseState.lX , __instance->mouseState.lY };
}
