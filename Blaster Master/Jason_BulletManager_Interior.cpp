#pragma once

#include "Jason_BulletManager_Interior.h"
#include "InteriorScene.h"
#include "Debug.h"

Jason_BulletManager::Jason_BulletManager() {
	bullets.clear();
}

void Jason_BulletManager::CheckBullet() {
	for (int i = 0; i < bullets.size(); i++)			
		if (bullets[i]->IsExploded() 
			|| dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GameObjectIsInScene(bullets[i]) == false)
			bullets.erase(bullets.begin() + i);
}

void Jason_BulletManager::Fire(int x, int y, int dx, int dy) {

	if (bullets.size() >= MAX_BULLET[level]) {
		return;
		DebugOut(L"WAIT\n");
	}
	else {
		BulletJasonInterior* newBullet = new BulletJasonInterior(dx, dy, level);
		newBullet->SetPosition(x, y);
		if (newBullet->Invalid()) {
			bullets.emplace_back(newBullet);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newBullet);
		}
	}
}

void Jason_BulletManager::CheckCheat() {
	if (DInput::KeyPress(DIK_P)) {
		int d = 0;
		if (DInput::KeyUp(DIK_1)) SetLevel(1), d++;
		else if (DInput::KeyUp(DIK_2)) SetLevel(2), d++;
		else if (DInput::KeyUp(DIK_3)) SetLevel(3), d++;
		else if (DInput::KeyUp(DIK_4)) SetLevel(4), d++;
		else if (DInput::KeyUp(DIK_5)) SetLevel(5), d++;
		else if (DInput::KeyUp(DIK_6)) SetLevel(6), d++;
		else if (DInput::KeyUp(DIK_7)) SetLevel(7), d++;
		else if (DInput::KeyUp(DIK_8)) SetLevel(8), d++;

		if (d) DebugOut(L"Gun Power Level changed to %d\n", level);
	}
}