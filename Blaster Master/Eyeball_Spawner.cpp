#pragma once

#include "Eyeball_Spawner.h"
#include "Eyeball_Linear.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Debug.h"

EyeballSpawner::EyeballSpawner(float left, float top, float right, float bottom) {
	roomColBox = FRECT(left, top, right, bottom);
}

void EyeballSpawner::Update(float dt) {
	coolDown -= dt;
	if (coolDown <= 0) {
		//spawn 4 or 5 Eyeballs 

		SetBoundary(Camera::GetInstance()->GetCollisionBox());
		coolDown = COOLDOWN;

		int eyeballNumber = 4 + rand() % 2; //4 or 5
		for (int i = 0; i < eyeballNumber; i++) {
			
			//determine where the eyeball will start
			float x, y;
			int direction = rand() % 4; //0 -> 3

			switch (static_cast<DIRECTION>(direction)) {
			case DIRECTION::GO_LEFT: {
				x = right; //opposite side
				y = RandRange(top, bottom);
				DebugOut(L"LEFT\n");
				break;
			}
			case DIRECTION::GO_UP: {
				x = RandRange(left, right);
				y = bottom; //opposite side
				DebugOut(L"UP\n");
				break;
			}
			case DIRECTION::GO_RIGHT: {
				x = left; //opposite side
				y = RandRange(top, bottom);
				DebugOut(L"RIGHT\n");
				break;
			}
			case DIRECTION::GO_DOWN: {
				x = RandRange(left, right);
				y = top; //opposite side
				DebugOut(L"DOWN\n");
				break;
			}
			}

			//create new eyeball
			EyeballLinear* eyeball = new EyeballLinear(x, y, arrDx[direction], arrDy[direction]);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(eyeball);

			DebugOut(L"ok %f %f %d %d\n", x, y, arrDx[direction], arrDy[direction]);
		}
	}
}

void EyeballSpawner::Render() {
	return;
}

void EyeballSpawner::SetBoundary(FRECT colBox) {
	left = max(colBox.left, roomColBox.left); 
	top = max(colBox.top, roomColBox.top);
	right = min(colBox.right, roomColBox.right);
	bottom = min(colBox.bottom, roomColBox.bottom);
}

float EyeballSpawner::RandRange(float l, float r) {
	int d = (int)(r - l + 2); //l -> r
	return (float)(l + (rand() % d));
}