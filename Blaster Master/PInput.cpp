#include "PInput.h"
#include "Debug.h"

PInput::PInput() {
	isPressed.resize(PINPUT_SIZE, false);
	firstPress.resize(PINPUT_SIZE, false);
}

PInput* PInput::__instance = NULL;

PInput* PInput::GetInstance() {
	if (__instance == NULL) 
		__instance = new PInput();

	return __instance;
}

void PInput::Press(int key) {
	//check if the key is pressed for the first time
	if (!firstPress[key] && !isPressed[key]) {
		firstPress[key] = true;
	}
	else {
		firstPress[key] = false;
	}

	//press the key anyway
	isPressed[key] = true;
}

void PInput::Release(int key) {
	//reset key booleans
	firstPress[key] = false;
	isPressed[key] = false;
}

void PInput::Update() {
	//convert from DInput to PInput
	for (int i = 0; i < PINPUT_SIZE; i++) {
		if (DInput::GetInstance()->KeyUp(Key[i]) || !DInput::GetInstance()->KeyPress(Key[i])) {
			PInput::GetInstance()->Release(i);
		}
		else if (DInput::GetInstance()->KeyPress(Key[i])) {
			PInput::GetInstance()->Press(i);
		}
	}
}

bool PInput::KeyPressed(int key) {
	if (PInput::GetInstance()->isPressed[key]) DebugOut(L"%d is pressed\n", key);
	return PInput::GetInstance()->isPressed[key];
}

bool PInput::KeyDown(int key) {
	if (PInput::GetInstance()->firstPress[key]) DebugOut(L"%d is down\n", key);
	return PInput::GetInstance()->firstPress[key];
}