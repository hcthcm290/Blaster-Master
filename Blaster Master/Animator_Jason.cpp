#pragma once
#include "Animator_Jason.h"
#include "StateConstant.h"
#include "Debug.h"

Animator_Jason::Animator_Jason() {
	this->AddAnimation(State::_JASON_IDLE_);
	this->AddAnimation(State::_JASON_WALK_);
	this->AddAnimation(State::_JASON_JUMP_);
	this->AddAnimation(State::_JASON_CLIMB_);
	this->AddAnimation(State::_JASON_CRAWL_);
	this->AddAnimation(State::_JASON_CMOVE_);
	this->AddAnimation(State::_JASON_DIE_);
}

void Animator_Jason::Draw(int animationID, float x, float y, bool flipX) {
	curFrame = animationSet[animationID]->Render(curFrame, x, y, flipX, 0, D3DCOLOR_XRGB(255, 255, 255), 0.6);
}