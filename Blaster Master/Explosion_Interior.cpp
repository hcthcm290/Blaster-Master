#pragma once
#include "Explosion_Interior.h"
#include "PlayScene.h"

ExplosionInterior::ExplosionInterior(int x, int y) {
	animator = new Animator();
	animator->AddAnimation(State::_EXPLOSION_INTERIOR_);
	this->SetPosition(x, y);
	this->SetVelocity(0, 0);
}

void ExplosionInterior::Update(float dt) {
	livingTime -= dt;
	if (livingTime <= 0) {
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		//delete this;
		return;
	}
	
	shakingWaitTime -= dt;
	if (shakingWaitTime <= 0) {
		shakingWaitTime = MAX_SHAKE_WAIT_TIME;
		int nx = -SHAKE_OFFSET + rand() % (SHAKE_OFFSET * 2 + 1);
		int ny = -SHAKE_OFFSET + rand() % (SHAKE_OFFSET * 2 + 1);
		this->SetPosition(x + nx, y + ny);
	}
}

void ExplosionInterior::Render() {
	animator->Draw(State::_EXPLOSION_INTERIOR_, x, y, false);
}