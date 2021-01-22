#include "ColliableBrick.h"
#include "Debug.h"
#include "PlayerItem.h"
#include "Sophia.h"
#include "PlayScene.h"
#include "BigJason.h"
#include "TheEye.h"
#include "Camera.h"
#include "CameraBoundaryLib.h"
#include "Sound.h"

PlayerItem::PlayerItem() {
	itemType = static_cast<ItemType>(rand() % 6 + 20001);
}

PlayerItem::PlayerItem(ItemType it) {
	itemType = it;
}

void PlayerItem::Update(float dt) {	
	if (livingTime <= 0) {
		//MakeNew();
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}
	else
	{
		CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		if (CollisionSystem::CheckOverlap(this, player))
		{
			Sound::getInstance()->play("PowerUp", false, 1);
			switch (itemType)
			{
			case Power: {
				DynamicObject* temp = dynamic_cast<DynamicObject*>(player);
				temp->HP += 10;
				if (temp->HP > temp->maxHP)
				{
					temp->HP = temp->maxHP;
					Sound::getInstance()->play("FullHeath", false, 1);
				}
				break; }
			case HomingMissle: {
				TheEye::GetInstance()->GetSophia()->Homing += 15;
				break; }
			case ThunderBreak: {
				TheEye::GetInstance()->GetSophia()->Thunder += 15;
				break; }
			case MultiWarhead: {
				TheEye::GetInstance()->GetSophia()->Rocket += 15;
				break; }
			case Gun: {
				if (dynamic_cast<BigJason*>(player) != nullptr)
					dynamic_cast<BigJason*>(player)->ChangeGunLevel(+1);
			break; }
			case BossDrop: {
				TheEye::GetInstance()->GetSophia()->isUpgraded = true;
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(player);
				player->SetPosition(8*16,85*16);
				Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));
				dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(player);
				break;
			}
			}
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		}
		livingTime -= dt;
	}
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
	/*if (dynamic_cast<Sophia*>(e.pGameObject) || dynamic_cast<ColliableBrick*>(e.pGameObject)) {
		if (dynamic_cast<Sophia*>(e.pGameObject))
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}*/
}

void PlayerItem::MakeNew() {
	PlayerItem* newPlayerItem = new PlayerItem();
	newPlayerItem->SetPosition(x + rand() % 100 - 50, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
}