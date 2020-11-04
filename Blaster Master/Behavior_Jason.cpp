#pragma once
#include "StateConstant.h"
#include "Behavior_Jason.h" 

/**
Behavior_Jason::Behavior_Jason(Jason* obj) {
	object = obj;
} 

void Behavior_Jason::Start() {
	object->SetState(State::_JASON_IDLE_);
}

void Behavior_Jason::Update(DWORD dt) {
	bool left = DInput::GetInstance()->KeyDown(DIK_A);
	bool right = DInput::GetInstance()->KeyDown(DIK_D);

	if (left || right) {
		object->SetState(State::_JASON_WALK_);
	}
	else {
		object->SetState(State::_JASON_IDLE_);
	}
} */