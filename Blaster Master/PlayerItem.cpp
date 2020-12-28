#include "ColliableBrick.h"
#include "Debug.h"
#include "PlayerItem.h"
#include "Sophia.h"
#include "PlayScene.h"
#include "BigJason.h"

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
			switch (itemType)
			{
			case Power: {
				DynamicObject* temp = dynamic_cast<DynamicObject*>(player);
				temp->HP += 10;
				if (temp->HP > temp->maxHP)
				{
					temp->HP = temp->maxHP;
				}
				break; }
			case HomingMissle: {
				for (auto temp : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
				{
					if(dynamic_cast<Sophia*>(temp) != nullptr)
						dynamic_cast<Sophia*>(temp)->Homing += 4;
					break;
				}
				break; }
			case ThunderBreak: {
				for (auto temp : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
				{
					if (dynamic_cast<Sophia*>(temp) != nullptr)
						dynamic_cast<Sophia*>(temp)->Thunder += 4;
					break;
				}
				break; }
			case MultiWarhead: {
				for (auto temp : dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetOnScreenObjs())
				{
					if (dynamic_cast<Sophia*>(temp) != nullptr)
						dynamic_cast<Sophia*>(temp)->Rocket += 4;
					break;
				}
				break; }
			case Gun: {
				
				if (dynamic_cast<BigJason*>(player) != nullptr)
					dynamic_cast<BigJason*>(player)->ChangeGunLevel(+1);
			break; }
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