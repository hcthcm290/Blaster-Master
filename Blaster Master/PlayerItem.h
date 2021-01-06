#pragma once
#include "DynamicObject.h"
#include "CollisionSystem.h"
#include "StateConstant.h"

enum ItemType {
	Power = State::_P_ITEM_POWER_,
	Hover = State::_P_ITEM_HOVER_,
	Gun = State::_P_ITEM_GUN_,
	HomingMissle = State::_P_ITEM_HOMING_MISSLE_,
	MultiWarhead = State::_P_ITEM_MULTI_WARHEAD_,
	ThunderBreak = State::_P_ITEM_THUNDER_BREAK_,
	BossDrop = 20007,
}; 

class PlayerItem : public DynamicObject
{
private:
	float livingTime = 15000; //15 second
	ItemType itemType;
	int iITemType = 0;

	void MakeNew();

public:
	PlayerItem();
	PlayerItem(ItemType it);
	void Update(float dt);
	void Render();
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
};
