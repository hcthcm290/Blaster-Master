#pragma once

#include "Bullet_Jason_Interior.h"
#include "Debug.h"
#include "Enemy.h"
#include "ColliableBrick.h"
#include "PlayScene.h"

BulletJasonInterior::BulletJasonInterior(int horizontal, int vertical, int level) {
	//if no horizontal either vertical, ignore this construction
	if ((vertical == 0 && horizontal == 0) || (vertical != 0 && horizontal !=0)) {
		DebugOut(L"BulletJasonInterior is invalid\n");
		return;
	}

	animator->AddAnimation(State::_BULLET_JASON_INTERIOR_);
	dx = horizontal; //-1(left) or 0(none) or 1(right)
	dy = vertical;	//-1(up) or 0(none) or 1(down)
	bulletLevel = level;
	damage = BASE_DAMAGE + 5 * (bulletLevel - 1); //draft damage, will be adjusted later

	if (bulletLevel <= 4) { //straight-fly bullets 1 -> 4
		flyPattern = new StraightPattern(livingTime, speed, bulletLevel, dx, dy);
	}
	else if (bulletLevel <= 6) { //circle-fly bullets 5 -> 6
		flyPattern = new CirclePattern(livingTime, speed, bulletLevel, dx, dy);
	}
	else if (bulletLevel <= 8) { //wave-fly bullets 7 -> 8 
		flyPattern = new WavePattern(livingTime, speed, bulletLevel, dx, dy);
	}
	else {
		DebugOut(L"BulletJasonInterior is invalid\n");
		return;
	}
}

void BulletJasonInterior::Update(float dt) {
	livingTime -= dt;
	if (livingTime <= 0) {
		Explode();
		return;
	}

	flyPattern->Update(dt);
	vx = dx*speed + flyPattern->GetVx();
	vy = dy*speed + flyPattern->GetVy();

}

void BulletJasonInterior::Render() {
	if (livingTime <= 0) return;

	//flipX = true ==> bullet faces right at default
	animator->Draw(State::_BULLET_JASON_INTERIOR_, x, y, true, GetRotation(), GetColor());
	DebugOut(L"Bullet is at %f %f \n", x, y);
}

FRECT BulletJasonInterior::GetCollisionBox() {
	return FRECT(x - 5, y - 4, x + 5, y + 4);
}

void BulletJasonInterior::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Enemy*>(e.pGameObject)) {
		if (bulletLevel == 8) //at level 8, bullets are not explode if collide with Enemy
			Explode();
	}
	else if (dynamic_cast<ColliableBrick*>(e.pGameObject)) {
		Explode(); //explode anyway :)
	}
}

void BulletJasonInterior::Explode() {
	//Add Explode GameObject
	DebugOut(L"Booom\n");
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

float BulletJasonInterior::GetRotation() {
	if (dynamic_cast<CirclePattern*>(flyPattern)) //angle due to position on circle
		return dynamic_cast<CirclePattern*>(flyPattern)->GetAngle();
	else {
		if (dy == 0) { //flying horizontal
			return 180 * (vx < 0);
		}
		else //flying vertical
		{
			return 90 * (dy > 0 ? 1 : -1);
		}
	}
}

D3DCOLOR BulletJasonInterior::GetColor() {
	if (bulletLevel == 8) { //at level 8, bullets have color-change effect
		//TODO: add color changing effect
		D3DCOLOR result = D3DCOLOR_ARGB(255, 255, 255, 255); //demo: return white
		return result;
	}
	//else return white color -> dont change color
	else return D3DCOLOR_ARGB(255, 255, 255, 255); 
}

#pragma region Fly Pattern 
//===	DEFAULT	===
void BulletJasonInteriorFlyPattern::Update(float dt) {
	//do nothing because it's virtual
}

//===	STARIGHT 1->4 ===
StraightPattern::StraightPattern(int& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED; //Bullets from level4 or higher will fly faster 
	livingTime = BASE_LIVINGTIME * (level >= 2 ? 99999 : 1); //all bullets which its level is higher than 2 is not disapear automatically

	DebugOut(L"Staright Boom\n");
}

//===	CIRCLE	5->6 ===
CirclePattern::CirclePattern(int& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED;
	livingTime = BASE_LIVINGTIME * 99999;

	angleOffset = BASE_ANGLEOFFSET * (level == 6 ? 2 : 1); //level6 Bullet will roll faster
	//bullets will go counter-clockwise, at first they head to the firing direction
	if (dx == 0) {
		vx = 0 * (dy < 0 ? -1 : 1);
		vy = speed;
		currentAngle = 90 * (dy < 0 ? -1 : 1); 
		//-1:UP(-90), 1:DOWN(90)
	}
	else { //dy == 0
		vx = speed * (dx < 0 ? -1 : 1);
		vy = 0;
		currentAngle = 180 * (dx < 0 ? 1 : 0);
		//0:LEFT(180), 1:RIGHT(0)
	}

	DebugOut(L"Circle Boom\n");
}

void CirclePattern::Update(float dt) {
	//update currentAngle for recording and GetAngle();
	currentAngle = (currentAngle + angleOffset * dt);
	if (currentAngle > 360) currentAngle -= 360;

	//calculate new vector
	float alpha = Deg2Rad(angleOffset * dt);
	float new_vx = vx*cos(alpha) - vy*sin(alpha);
	float new_vy = vy*cos(alpha) + vx*sin(alpha);

	//update new variables
	vx = new_vx;
	vy = new_vy;
}

//===	WAVE 7->8	===
WavePattern::WavePattern(int& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED;
	livingTime = BASE_LIVINGTIME * 99999; 

	isHorizontal = (dx != 0); //if not horizontal, definately veritcal
	fiOffset = BASE_FI_OFFSET * (level == 8 ? 2 : 1); //level8 bullets wave faster

	DebugOut(L"Wave Boom\n");
}

void WavePattern::Update(float dt) {
	fi += fiOffset * dt;
	if (isHorizontal) { //flying horizontal - adjust vy
		vx = 0;
		vy = cos(fi) * BASE_SPEED; //cos(fi) la dao ham cua sin(fi)
	}
	else { //flying vertical - adjust vx
		vx = cos(fi) * BASE_SPEED;
		vy = 0;
	}
}
#pragma endregion