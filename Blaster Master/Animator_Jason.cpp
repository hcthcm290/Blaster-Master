#pragma once
#include "Animator_Jason.h"
#include "StateConstant.h"

Animator_Jason::Animator_Jason() {
	AddAnimation(State::_JASON_IDLE_);
	AddAnimation(State::_JASON_WALK_);
	AddAnimation(State::_JASON_JUMP_);
	AddAnimation(State::_JASON_CLIMB_);
	AddAnimation(State::_JASON_CRAWL_);
	AddAnimation(State::_JASON_CMOVE_);
	AddAnimation(State::_JASON_DIE_);
}

void Animator_Jason::Draw(int animationID, float x, float y, bool flipX) {
	curFrame = animationSet[animationID]->Render(curFrame, x, y, flipX);
}