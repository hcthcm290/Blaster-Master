#pragma once
#include "Bullet_Eyeball.h"
#include "InteriorScene.h"
#include "Playable.h"
#include "ColliableBrick.h"
#include "Explosion_Interior.h"

Bullet_Eyeball::Bullet_Eyeball(float x, float y) {
	animator = new Animator();
	animator->AddAnimation(State::_EYEBALL_BULLET_);

	this->SetPosition(x, y);

	float dx = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().x - x;
	float dy = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition().y - y;
	
	//avoid zero
	if (dx == 0) dx = 1;
	if (dy == 0) dy = 1;

	vx0 = (dx * SPEED) / sqrt(dx * dx + dy * dy);
	vy0 = dy * vx0 / dx;

	vx0 *= (vx0 * dx >= 0 ? 1 : -1);
	vy0 *= (vy0 * dy >= 0 ? 1 : -1);
}

void Bullet_Eyeball::Update(float dt) {
	if (livingTime < 0)
	{
		Explode();
		return;
	}
	else
	{
		livingTime -= dt*1000;
	}
	CGameObject* player = dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(2);
		Explode();
	}
	SetVelocity(vx0 * dt, vy0 * dt);
}

void Bullet_Eyeball::Render() {
	animator->Draw(State::_EYEBALL_BULLET_, x, y, false);
}

FRECT Bullet_Eyeball::GetCollisionBox() {
	return FRECT(x - 4, y - 4, x + 4, y + 4);
}

void Bullet_Eyeball::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL) {
		Explode();
	}
}

void Bullet_Eyeball::Explode() {
	//Add Explode GameObject
	ExplosionInterior* explosion = new ExplosionInterior(x + vx / 16, y + vy / 16);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}