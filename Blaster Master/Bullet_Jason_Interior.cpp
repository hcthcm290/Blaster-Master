#pragma once

#include "Bullet_Jason_Interior.h"
#include "Debug.h"
#include "Enemy.h"
#include "ColliableBrick.h"
#include "PlayScene.h"
#include "Explosion_Interior.h"
#include "Rock.h"

BulletJasonInterior::BulletJasonInterior(int dx, int dy, int level) {
	//if no horizontal either vertical, ignore this construction
	if ((dy == 0 && dx == 0) || (dy != 0 && dx !=0)) {
		DebugOut(L"BulletJasonInterior is invalid\n");
		dx = 1;
		dy = 0;
		//return;
	}

	animator->AddAnimation(State::_BULLET_JASON_INTERIOR_);
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
	vx = speed * flyPattern->GetVx();
	vy = speed * flyPattern->GetVy();

	//decreasing colorDelay for changing-color effect (apply in GetColor() )
	colorDelay -= dt;
}

void BulletJasonInterior::Render() {
	if (livingTime <= 0) return;

	//flipX = false ==> bullet faces left at default
	animator->Draw(State::_BULLET_JASON_INTERIOR_, x, y, false, 180 + GetRotation(), GetColor());
}

FRECT BulletJasonInterior::GetCollisionBox() {
	return FRECT(x - 5, y - 4, x + 5, y + 4);
}

void BulletJasonInterior::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Enemy*>(e.pGameObject)) {
		dynamic_cast<DynamicObject*>(e.pGameObject)->TakeDamage(10);
		if (bulletLevel != 8) //at level 8, bullets are not explode if collide with Enemy
			Explode();
	}
	else if (dynamic_cast<ColliableBrick*>(e.pGameObject)) {
		if(dynamic_cast<Rock*>(e.pGameObject))
			dynamic_cast<Rock*>(e.pGameObject)->TakeDamage(10);
		Explode(); //explode anyway :)
	}
}

void BulletJasonInterior::Explode() {
	//Add Explode GameObject
	ExplosionInterior* explosion = new ExplosionInterior(x + vx / 16, y + vy / 16);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	exploded = true;
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
	if (bulletLevel == 8 && colorDelay <= 0) { //at level 8, bullets have color-change effect
		if (color == 7) color = rand() % 7;
		else color = 7;

		colorDelay = COLOR_DELAY;
	}
	//else return white color -> dont change color
	return arrColor[color];
}

#pragma region Fly Pattern 
//===	DEFAULT	===
void BulletJasonInteriorFlyPattern::Update(float dt) {
	//do nothing because it's virtual
}

//===	STARIGHT 1->4 ===
StraightPattern::StraightPattern(float& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED; 
	livingTime = BASE_LIVINGTIME * (level >= 2 ? 99999 : 1); //all bullets which its level is higher than 2 is not disapear automatically
	
	if (dx == 0) vx = 0;
	else vx = (dx < 0 ? -1 : 1);

	if (dy == 0) vy = 0;
	else vy = (dy < 0 ? -1 : 1);
}

//===	CIRCLE	5->6 ===
CirclePattern::CirclePattern(float& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED * (level == 6 ? 1 : 0.5);
	livingTime = 5; //5s

	angleOffset = BASE_ANGLEOFFSET * (level == 6 ? 2 : 2); //level6 Bullet will roll faster
	//bullets will go counter-clockwise, at first they head to the firing direction
	if (dx == 0) {
		vx = 0;
		vy = (dy < 0 ? -1 : 1);
		currentAngle = 90 * (dy < 0 ? -1 : 1);
		//-1:UP(-90), 1:DOWN(90)
	}
	else { //dy == 0
		vx = (dx < 0 ? -1 : 1);
		vy = 0;
		currentAngle = 180 * (dx < 0 ? 1 : 0);
		//0:LEFT(180), 1:RIGHT(0)
	}

	direction = arrDirection[iDirection];
	iDirection = (iDirection + 1) % 4;
}

int CirclePattern::iDirection = 0;
int CirclePattern::arrDirection[4] = { 0,1,0,-1 };

void CirclePattern::Update(float dt) {
	//update currentAngle for recording and GetAngle();
	float degAlpha = angleOffset * dt * direction;
	currentAngle += degAlpha;

	//calculate new vector
	float alpha = Deg2Rad(degAlpha);
	float new_vx = vx*cos(alpha) - vy*sin(alpha);
	float new_vy = vy*cos(alpha) + vx*sin(alpha);

	//update new variables
	vx = new_vx;
	vy = new_vy;
}

float CirclePattern::GetAngle() {
	float result = currentAngle;
	while (result >= 360) result -= 360;
	while (result < 0) result += 360;

	return result;
}

//===	WAVE 7->8	===
WavePattern::WavePattern(float& livingTime, int& speed, int level, int dx, int dy) {
	speed = BASE_SPEED * 0.5;
	livingTime = BASE_LIVINGTIME * 99999; 

	if (dx == 0) vx = 0; else vx = (dx < 0 ? -1 : 1);
	if (dy == 0) vy = 0; else vy = (dy < 0 ? -1 : 1);
	isHorizontal = (dx != 0); //if not horizontal, definately veritcal
	fiOffset = BASE_FI_OFFSET; //level8 bullets wave faster

	//get new delay
	delay = nextDelay;
	nextDelay += DELAY_WAVE_OFFSET;
	if (nextDelay > DELAY_WAVE_MAX) nextDelay = 0;

	//get new direction
	direction = nextDirection;
	nextDirection = -nextDirection;

	//level 8 adjustment
	if (level == 8) {
		fiOffset *= 2;
		direction *= 2;
		spreadDiv = 1.25; //no shrink
	}
}

float WavePattern::nextDelay = 0;
float WavePattern::nextDirection = 1;

void WavePattern::Update(float dt) {
	//if the bullet is "delaying", it flies straight
	delay -= dt;
	if (delay > 0) return;

	fi += fiOffset * dt;
	if (abs(sin(fi)) <= dt) {	//the height/width shrinks every time it reach center point
		spread /= spreadDiv;
	}

	if (isHorizontal) { //flying horizontal - adjust vy
		vy = cos(fi) * spread * direction; //cos(fi) la dao ham cua sin(fi)
		
	}
	else { //flying vertical - adjust vx
		vx = cos(fi) * spread * direction;
		
	}
}
#pragma endregion