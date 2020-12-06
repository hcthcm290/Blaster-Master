#pragma once
#include "Eyeball_Linear.h"
#include "PlayScene.h"
#include "Explosion_Interior.h"

EyeballLinear::EyeballLinear(float x, float y, int dx, int dy) {
	this->SetPosition(x, y);
	livingTime = LIVING_TIME;

	int iDeg = rand() % 46; //0 -> 45
	int iDirection = (rand() % 2 ? 1 : -1); // -1 or 1
	int iInitAngle;

	if (dx != 0) { //go horizontal
		if (dx < 0) { //go left
			iInitAngle = 180;
		}
		else { //go right 
			iInitAngle = 0;
		}
	}
	else { //go vertical
		if (dy < 0) { //go up
			iInitAngle = 90;
		}
		else {	//go down
			iInitAngle = -90;
		}
	}

	float angle = Deg2Rad(iDeg * iDirection + iInitAngle);
	vx0 = cos(Deg2Rad(angle));
	vy0 = -sin(Deg2Rad(angle)); //y-axis is flipped
}


void EyeballLinear::Update(float dt)
{
	vx = vx0 * dt;
	vy = vy0 * dt;

	livingTime -= dt;
	if (livingTime <= 0)
		Die();
}

void EyeballLinear::Render()
{
	animator->Draw(State::_EYEBALL_, x, y, false);
}

FRECT EyeballLinear::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + 8);
}

void EyeballLinear::OnCollisionEnter(CollisionEvent e) {
	//if the Eyeball collide the second time with specific objects, it dies
	if (dynamic_cast<ColliableBrick*>(e.pGameObject)) {
		if (brick == NULL || brick == e.pGameObject) {
			brick = dynamic_cast<ColliableBrick*>(e.pGameObject); //nothing happend
		}
		else Die();
	}
}

void EyeballLinear::Die() {
	ExplosionInterior* explosion = new ExplosionInterior(x, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}