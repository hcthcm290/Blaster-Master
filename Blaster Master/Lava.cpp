#include "Lava.h"
#include "StateConstant.h"
#include "PlayScene.h"
#include "CollisionSystem.h"

Lava::Lava(int length) {
	animator = new Animator();
	animator->AddAnimation(State::_LAVA_);
	this->length = length;
}

void Lava::Update(float dt) {
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(20);
		//ex = true;
	}
	flipCountDown -= dt;
	if (flipCountDown <= 0) {
		flipCountDown = 20;
		flipX = !flipX;
	}
}

void Lava::Render() {
	for (int i = 0; i < length; i++) {
		animator->Draw(State::_LAVA_, x + i * 16, y, flipX);
	}
}

FRECT Lava::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + length * 16 - 8, y + 8);
}

void Lava::OnCollisionEnter(CollisionEvent e) {

}