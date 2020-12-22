#pragma once

#include "Boss.h"
#include "ColliableBrick.h"
#include "InteriorScene.h"
#include "Playable.h"
#include "Debug.h"

#pragma region Boss
/** ===== BOSS ===== **/

Boss::Boss(float x, float y) {
	//set position first because Hands is initialized due to it
	this->SetPosition(x, y);

	//set boss' health
	this->SetMaxHP(BOSS_MAX_HEALTH);
	this->SetCurrentHP(BOSS_MAX_HEALTH);

	//random velocity
	vx0 = BOSS_SPEED * (rand() % 2 ? 1 : -1);
	vy0 = BOSS_SPEED * (rand() % 2 ? 1 : -1);

	//animator initialization
	animator = new Animator();
	animator->AddAnimation(State::_BOSS_BODY_);

	//initiate Boss Hands
	float arrShoulderX[] = { rShoulderX(), lShoulderX() };
	for (int i = 0; i < 2; i++) {
		arrBossHand[i] = new BossHand(arrShoulderX[i], y);
		//dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(arrBossHand[i]);
	}
}

void Boss::Update(float dt) {
	CheckFire(dt);
	SetVelocity(vx0 * dt, vy0 * dt);

	//hand control
	float arrShoulderX[] = { rShoulderX(), lShoulderX() };
	for (int i = 0; i < 2; i++) {
		arrBossHand[i]->SetNextPosition(arrShoulderX[i], y, i);
		arrBossHand[i]->Update(dt);
	}
}

void Boss::Render() {
	animator->Draw(State::_BOSS_BODY_, x, y, false);
	//DebugOut(L"Body: %f %f\n", x, y);
	//draw BossArms and BossHands
	for (int i = 0; i < 2; i++) {
		arrBossHand[i]->Render(i);
		//DebugOut(L"%d hand: %f %f\n", i, arrBossHand[i]->GetPosition().x, arrBossHand[i]->GetPosition().y);
	}
}

void Boss::CheckFire(float dt) {
	fireTimer -= dt;
	if (fireTimer <= 0) {
		switch (bulletRemaining)
		{
			case 0: //reloading
			{
				bulletRemaining = 4;
				fireTimer = BOSS_FIRE_COOLDOWN;
				break;
			}
			default: //fire
			{ 
				bulletRemaining--;
				Fire();
				fireTimer = BOSS_FIRE_SPEED;
				break;
			}
		}
	}
		
}

void Boss::Fire() {
	Bullet_Eyeball* bullet = new Bullet_Eyeball(GetPosition().x, GetPosition().y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
}

FRECT Boss::GetCollisionBox() {
	return FRECT(x - 32, y - 32, x + 32, y + 32); //64 x 64
}

void Boss::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL) {
		//do bounce effect if touch walls
		if (e.nx != 0) vx0 *= -1;
		else vy0 *= -1;
	}
}
#pragma endregion

#pragma region BOSS_ARM
/** ===== BOSS ARM ===== **/

BossArm::BossArm() {
	//Animator initialization
	animator = new Animator();
	animator->AddAnimation(State::_BOSS_ARM_);
}

void BossArm::Update(float dt) {
	//avoid object shaking 
	float X = (abs(vx0 * dt) >= MIN_MOVEMENT_VELOCITY ? vx0 * dt : 0);
	float Y = (abs(vy0 * dt) >= MIN_MOVEMENT_VELOCITY ? vy0 * dt : 0);
	SetVelocity(X, Y);
}

void BossArm::Render() {}

void BossArm::Render(bool flipX) {
	animator->Draw(State::_BOSS_ARM_, x, y , flipX);
}

FRECT BossArm::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + 8);
}

void BossArm::OnCollisionEnter(CollisionEvent e) {

}

void BossArm::SetDestination(float newX, float newY) {
	x = newX;
	y = newY;
	/*float dx = newX - x;
	float dy = newY - y;

	if (dx == 0) dx = 1;
	if (dy == 0) dy = 1;

	vx0 = (dx * ARM_SPEED) / sqrt(dx * dx + dy * dy);
	vy0 = dy * vx0 / dx;

	vx0 *= (vx0 * dx >= 0 ? 1 : -1);
	vy0 *= (vy0 * dy >= 0 ? 1 : -1);*/
}
#pragma endregion

#pragma region BOSS_HAND
/** ===== BOSS HAND ===== **/

BossHand::BossHand(float x, float y) {
	directionX = 1;
	directionY = 1;

	freezeX = GetTickCount();
	freezeY = GetTickCount();
	//set position first because Hands is initialized due to it
	this->SetPosition(x, y);

	//Animator initialization
	animator = new Animator();
	animator->AddAnimation(State::_BOSS_HAND_);

	//initiate Boss Arms
	for (int i = 0; i < 4; i++) {
		arrBossArm[i] = new BossArm();
		arrBossArm[i]->SetPosition(x, y);
		dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(arrBossArm[i]);
	}
}

void BossHand::Update(float dt) {
	//float X = vx0;//(abs(vx0 * dt) >= MIN_MOVEMENT_VELOCITY ? vx0 * dt : 0);
	//float Y = vy0;//(abs(vy0 * dt) >= MIN_MOVEMENT_VELOCITY ? vy0 * dt : 0);
	//SetVelocity(X, Y);
	x += dt * vx;
	y += dt * vy;
	CGameObject* player = dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(2);
	}
}

void BossHand::Render() {}

void BossHand::Render(bool flipX) {
	for (int i = 0; i < 4; i++) {
		arrBossArm[i]->Render(flipX);
	}
	animator->Draw(State::_BOSS_HAND_, x , y + 6, flipX);
}

FRECT BossHand::GetCollisionBox() { //Hand is a bit below its center
	return FRECT(x - 9, y - 10, x + 9, y + 20);
}

void BossHand::OnCollisionEnter(CollisionEvent e) {
	/*if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL) {
		if (e.nx != 0) vx0 *= -1;
		else vy0 *= -1;
	}*/
}

void BossHand::SetNextPosition(float shoudlerX, float shoudlerY, bool isLeft) {
	////Left Shoulder can only roam on the left, same to the Right Shoulder
	//float posX = shoudlerX + DISTANCE * offsetX * (isLeft ? -1 : 1);
	//float posY = shoudlerY + DISTANCE * offsetY;

	//if ((abs(x - posX) <= EPSILON && abs(y - posY) <= EPSILON)
	//	|| (GetDistance(shoudlerX, shoudlerY, x, y) >= DISTANCE)) {
	//	//set new offsets
	//	offsetX = RandRange(0, 1);
	//	offsetY = RandRange(-1, 1);

	//	//re-calculate posX, posY
	//	posX = shoudlerX + DISTANCE * offsetX * (isLeft ? -1 : 1);
	//	posY = shoudlerY + DISTANCE * offsetY;
	//}

	////DebugOut(L"posX: %f  posY: %f\n", posX, posY);

	//float dx = posX - x;
	//float dy = posY - y;

	//vx0 = (dx * ARM_SPEED) / sqrt(dx * dx + dy * dy);
	//vy0 = dy * vx0 / dx;

	//vx0 *= (vx0 * dx >= 0 ? 1 : -1);
	//vy0 *= (vy0 * dy >= 0 ? 1 : -1);
	if (!isLeft)
	{
		if (x >= shoudlerX + 100)
		{
			x = shoudlerX + 100;
			if (!isFreezeX)
			{
				isFreezeX = true;
				freezeX = GetTickCount();
				vx = 0;
				vy = 60 * directionY;
			}
			if(GetTickCount() - freezeX > 1000)
			{
				vx = -100;
				directionX = -1;
			}
		}
		else
		{
			if (x <= shoudlerX - 20)
			{
				x = shoudlerX - 20;
				if (!isFreezeX)
				{
					isFreezeX = true;
					freezeX = GetTickCount();
					vx = 0;
					vy = 60 * directionY;
				}
				if (GetTickCount() - freezeX > 1000)
				{
					isFreezeX = false;
					vx = 100;
					directionX = 1;
				}
			}
			else
			{
				if (GetTickCount() - freezeX > 1000)
				{
					vx = 60 * directionX;
					isFreezeX = false;
				}
				
			}
		}
		if (y >= shoudlerY + 58)
		{
			y = shoudlerY + 58;
			if (!isFreezeY)
			{
				isFreezeY = true;
				freezeY = GetTickCount();
				vy = 0;
				vx = 60 * directionX;
			}
			if (GetTickCount() - freezeY > 1000)
			{
				isFreezeY = false;
				vy = -100;
				directionY = -1;
			}
		}
		else
		{
			if (y <= shoudlerY - 58)
			{
				y = shoudlerY - 58;
				if (!isFreezeY)
				{
					isFreezeY = true;
					freezeY = GetTickCount();
					vy = 0;
					vx = 60 * directionX;
				}
				if (GetTickCount() - freezeY > 1000)
				{
					isFreezeY = false;
					vy = 100;
					directionY = 1;
				}
			}
			else
			{
				if (GetTickCount() - freezeY > 1000)
				{
					vy = 60 * directionY;
					isFreezeY = false;
				}
				
			}
		}
	}
	else
	{
		if (x >= shoudlerX + 20)
		{
			x = shoudlerX + 20;
			if (!isFreezeX)
			{
				isFreezeX = true;
				freezeX = GetTickCount();
				vx = 0;
			}
			if (GetTickCount() - freezeX > 500)
			{
				vx = -100;
				directionX = -1;
			}
		}
		else
		{
			if (x <= shoudlerX - 100)
			{
				x = shoudlerX - 100;
				if (!isFreezeX)
				{
					isFreezeX = true;
					freezeX = GetTickCount();
					vx = 0;
				}
				if (GetTickCount() - freezeX > 500)
				{
					isFreezeX = false;
					vx = 100;
					directionX = 1;
				}
			}
			else
			{
				vx = 60 * directionX;
			}
		}
		if (y >= shoudlerY + 58)
		{
			y = shoudlerY + 58;
			if (!isFreezeY)
			{
				isFreezeY = true;
				freezeY = GetTickCount();
				vy = 0;
			}
			if (GetTickCount() - freezeY > 500)
			{
				isFreezeY = false;
				vy = -100;
				directionY = -1;
			}
		}
		else
		{
			if (y <= shoudlerY - 58)
			{
				y <= shoudlerY - 58;
				if (!isFreezeY)
				{
					isFreezeY = true;
					freezeY = GetTickCount();
					vy = 0;
				}
				if (GetTickCount() - freezeY > 500)
				{
					isFreezeY = false;
					vy = 100;
					directionY = 1;
				}
			}
			else
			{
				vy = 60 * directionY;
			}
		}
	}
	
	UpdateArms(shoudlerX, shoudlerY);
}

void BossHand::UpdateArms(float sx, float sy) {
	float dx = (x - sx) / 5;
	float dy = (y - sy) / 5;

	for (int i = 0; i < 4; i++) {
		arrBossArm[i]->SetDestination(sx + dx * (i + 1), sy + dy * (i + 1));
	}
}
#pragma endregion