#include "Bullet_Jason.h"
#include "Animator_Jason.h"
#include "Debug.h"

#include "Jason.h"
#include "PlayScene.h"

Bullet_Jason::Bullet_Jason(int horizontalDirection, float xs, float ys) {
	animator = new Animator();
	animator->AddAnimation(State::_BULLET_JASON_);

	vx = horizontalDirection * speed;
	vy = 0;
	x = xs;
	y = ys;
}

void Bullet_Jason::Update(float dt)
{
	if (livingTime <= 0) 
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	else 
		livingTime -= dt;
}

void Bullet_Jason::Render()
{
	if (livingTime <= 0) return;
	animator->Draw(State::_BULLET_JASON_, x, y, false);
}

FRECT Bullet_Jason::GetCollisionBox() {
	return FRECT(x - 4, y - 4, x + 4, y + 4);
}

void Bullet_Jason::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Jason*>(e.pGameObject) || dynamic_cast<Bullet_Jason*>(e.pGameObject)) {
		//do nothing
	}
	else {
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
}