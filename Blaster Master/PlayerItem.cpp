#include "ColliableBrick.h"
#include "Debug.h"
#include "PlayerItem.h"
#include "Sophia.h"
#include "PlayScene.h"

PlayerItem::PlayerItem() {
	itemType = static_cast<ItemType>(rand() % 6 + 20001);
}

PlayerItem::PlayerItem(ItemType it) {
	itemType = it;
}

void PlayerItem::Update(float dt) {	
	if (livingTime <= 0) {
		MakeNew();
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
	else livingTime -= dt;
}

void PlayerItem::Render() {
	if (iITemType == 0) {
		animator = new Animator();
		iITemType = static_cast<int>(itemType);
		animator->AddAnimation(iITemType);
	}
	else animator->Draw(iITemType, x, y, false);
}

FRECT PlayerItem::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + 8);
}

void PlayerItem::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Sophia*>(e.pGameObject) || dynamic_cast<Jason*>(e.pGameObject)) {
		switch (itemType) {
		case Power:
			//Power UP
			break;
		case Hover:
			//Hover UP
			break;
		case Gun:
			//Gun UP
			break; 
		case HomingMissle:
			//HM UP
			break;
		case MultiWarhead:
			//MW UP
			break;
		case ThunderBreak:
			//TB UP
			break;
		}
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
}

void PlayerItem::MakeNew() {
	PlayerItem* newPlayerItem = new PlayerItem();
	newPlayerItem->SetPosition(x + rand() % 100 - 50, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
}