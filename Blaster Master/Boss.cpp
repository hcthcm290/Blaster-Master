#pragma once

#include "Boss.h"
#include "ColliableBrick.h"
#include "InteriorScene.h"
#include "Playable.h"
#include "Debug.h"
#include "VisionBox.h"
#include "PlayerItem.h"

#pragma region Boss
/** ===== BOSS ===== **/

Boss::Boss(float x, float y) {
	CGameObject* obj = new VisionBox(48*16,63*16,10*16,10*16+2);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);

	lastex = GetTickCount();
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
	animator->AddAnimation(28203);

	//initiate Boss Hands
	float arrShoulderX[] = { rShoulderX(), lShoulderX() };
	for (int i = 0; i < 2; i++) {
		arrBossHand[i] = new BossHand(arrShoulderX[i], y - 5);
		//dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(arrBossHand[i]);
	}
}

void Boss::Update(float dt) {
	if (dead)
	{
		SetVelocity(0, 0);
		if (GetTickCount() - deadTime > 5000)
		{
			//dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this->arrBossHand[0]);
			//dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this->arrBossHand[1]);
			PlayerItem* newPlayerItem = new PlayerItem(BossDrop);
			newPlayerItem->SetPosition(x, y);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
			dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->SetFightingBoss(false);
		}
		return;
	}
	if (HP <= 0)
	{
		dead = true;
		deadTime = GetTickCount();
	}
	CheckFire(dt);
	SetVelocity(vx0 * dt, vy0 * dt);

	//hand control
	float arrShoulderX[] = { rShoulderX(), lShoulderX() };
	for (int i = 0; i < 2; i++) {
		arrBossHand[i]->SetNextPosition(arrShoulderX[i], y - 5, i);
		arrBossHand[i]->inv = this->inv;
		arrBossHand[i]->Update(dt);
	}
}

void Boss::Render() {
	if (dead)
	{
		if (GetTickCount() - last_blink > 300)
		{
			switch (inv)
			{
			case 5: inv = 4; break;
			case 4: inv = 3; break;
			case 3: inv = 2; break;
			case 2: inv = 1; break;
			case 1: inv = 0; break;
			case 0: inv = 5; break;
			default: inv = 5; break;
			}
			//currentColor = 5 - currentColor;
			last_blink = GetTickCount();
		}
		//srand((int)time(0));
		if (GetTickCount() - lastex > 200)
		{
			int tempx = x + rand() % 100 - 50;
			int tempy = y + rand() % 100 - 50;
			CGameObject* obj = new ExplosiveBoss(tempx, tempy);
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(obj);
			lastex = GetTickCount();
		}
		for (int i = 0; i < 2; i++) {
			arrBossHand[i]->inv = this->inv;
			arrBossHand[i]->dead = true;
			arrBossHand[i]->Render(i);
			//DebugOut(L"%d hand: %f %f\n", i, arrBossHand[i]->GetPosition().x, arrBossHand[i]->GetPosition().y);
		}
		animator->Draw(28203, x, y, false, 0, deadColor[inv]);
		
		return;
	}
	if (inv != -1) {
		animator->Draw(State::_BOSS_BODY_, x, y, false, 0, Color[inv]);
		if (GetTickCount64() - last_blink >= 50) {
			if (GetTickCount64() - startTakeDamage > 150)
			{
				inv = -1;
			}
			else
			{
				last_blink = GetTickCount64();
				switch (inv)
				{
				case 1: inv = 0; break;
				case 0: inv = 1; break;
				}
			}
		}
	}
	else
	{
		animator->Draw(State::_BOSS_BODY_, x, y, false, 0);
	}
	//DebugOut(L"Body: %f %f\n", x, y);
	//draw BossArms and BossHands
	for (int i = 0; i < 2; i++) {
		arrBossHand[i]->Render(i);
		//DebugOut(L"%d hand: %f %f\n", i, arrBossHand[i]->GetPosition().x, arrBossHand[i]->GetPosition().y);
	}
}

void Boss::TakeDamage(int dmg)
{
	this->HP -= dmg;
	startTakeDamage = GetTickCount();
	last_blink = GetTickCount();
	inv = 1;
	if (HP < 0)
	{
		HP = 0;
	}
	if (HP == 0)
	{
		/*auto explode = new Explosive();
		explode->SetPosition(x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
		if (this != dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer())
			SoundManager::GetInstance()->PlaySoundW("EnemyDie.wav");
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);*/
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
	if (dynamic_cast<VisionBox*>(e.pGameObject) != NULL) {
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
	if (dead)
	{
		return;
	}
	//avoid object shaking 
	float X = (abs(vx0 * dt) >= MIN_MOVEMENT_VELOCITY ? vx0 * dt : 0);
	float Y = (abs(vy0 * dt) >= MIN_MOVEMENT_VELOCITY ? vy0 * dt : 0);
	SetVelocity(X, Y);
}

void BossArm::Render() {}

void BossArm::Render(bool flipX) {
	if (dead)
	{
		animator->Draw(State::_BOSS_ARM_, x, y, flipX, 0, deadColor[inv]);
		return;
	}
	if (inv != -1) {
		animator->Draw(State::_BOSS_ARM_, x, y, flipX, 0, Color[inv]);
	}
	else
	{
		animator->Draw(State::_BOSS_ARM_, x, y, flipX);
	}
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
	if (dead)
	{
		return;
	}
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
	if (dead) {
		animator->Draw(State::_BOSS_HAND_, x, y + 6, flipX, 0, deadColor[inv]);
		for (int i = 0; i < 4; i++) {
			arrBossArm[i]->dead = true;
			arrBossArm[i]->inv = this->inv;
			arrBossArm[i]->Render(flipX);
		}
		return;
	}
	for (int i = 0; i < 4; i++) {
		arrBossArm[i]->Render(flipX);
	}
	if (inv != -1) {
		animator->Draw(State::_BOSS_HAND_, x, y + 6, flipX, 0, Color[inv]);
	}
	else
	{
		animator->Draw(State::_BOSS_HAND_, x, y + 6, flipX);
	}
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
		if (x >= shoudlerX + 60)
		{
			x = shoudlerX + 60;
			if (!isFreezeX)
			{
				isFreezeX = true;
				freezeX = GetTickCount();
				vx = 0;
				vy = 100 * directionY;
			}
			if(GetTickCount() - freezeX > 600)
			{
				vx = -100;
				directionX = -1;
			}
		}
		else
		{
			if (x <= shoudlerX - 10)
			{
				x = shoudlerX - 10;
				if (!isFreezeX)
				{
					isFreezeX = true;
					freezeX = GetTickCount();
					vx = 0;
					vy = 100 * directionY;
				}
				if (GetTickCount() - freezeX > 600)
				{
					isFreezeX = false;
					vx = 100;
					directionX = 1;
				}
			}
			else
			{
				if (GetTickCount() - freezeX > 600)
				{
					vx = 100 * directionX;
					isFreezeX = false;
				}
				
			}
		}
		if (y >= shoudlerY + 70)
		{
			y = shoudlerY + 70;
			if (!isFreezeY)
			{
				isFreezeY = true;
				freezeY = GetTickCount();
				vy = 0;
				vx = 100 * directionX;
			}
			if (GetTickCount() - freezeY > 600)
			{
				isFreezeY = false;
				vy = -100;
				directionY = -1;
			}
		}
		else
		{
			if (y <= shoudlerY - 70)
			{
				y = shoudlerY - 70;
				if (!isFreezeY)
				{
					isFreezeY = true;
					freezeY = GetTickCount();
					vy = 0;
					vx = 100 * directionX;
				}
				if (GetTickCount() - freezeY > 600)
				{
					isFreezeY = false;
					vy = 100;
					directionY = 1;
				}
			}
			else
			{
				if (GetTickCount() - freezeY > 600)
				{
					vy = 100 * directionY;
					isFreezeY = false;
				}

			}
		}
	}
	else
	{
		if (x >= shoudlerX + 10)
		{
			x = shoudlerX + 10;
			if (!isFreezeX)
			{
				isFreezeX = true;
				freezeX = GetTickCount();
				vx = 0;
				vy = 100 * directionY;
			}
			if (GetTickCount() - freezeX > 500)
			{
				vx = -100;
				directionX = -1;
			}
		}
		else
		{
			if (x <= shoudlerX - 60)
			{
				x = shoudlerX - 60;
				if (!isFreezeX)
				{
					isFreezeX = true;
					freezeX = GetTickCount();
					vx = 0;
					vy = 100 * directionY;
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
				if (GetTickCount() - freezeX > 600)
				{
					vx = 100 * directionX;
					isFreezeX = false;
				}
			}
		}
		if (y >= shoudlerY + 70)
		{
			y = shoudlerY + 70;
			if (!isFreezeY)
			{
				isFreezeY = true;
				freezeY = GetTickCount();
				vy = 0;
				vx = 100 * directionX;
			}
			if (GetTickCount() - freezeY > 600)
			{
				isFreezeY = false;
				vy = -100;
				directionY = -1;
			}
		}
		else
		{
			if (y <= shoudlerY - 70)
			{
				y = shoudlerY - 70;
				if (!isFreezeY)
				{
					isFreezeY = true;
					freezeY = GetTickCount();
					vy = 0;
					vx = 100 * directionX;
				}
				if (GetTickCount() - freezeY > 600)
				{
					isFreezeY = false;
					vy = 100;
					directionY = 1;
				}
			}
			else
			{
				if (GetTickCount() - freezeY > 600)
				{
					vy = 100 * directionY;
					isFreezeY = false;
				}

			}
		}
	}
	
	UpdateArms(shoudlerX, shoudlerY);
}

void BossHand::UpdateArms(float sx, float sy) {
	float dx = (x - sx) / 5;
	float dy = (y - sy) / 5;

	ChainArm(arrBossArm[3], this);
	for (int i = 3; i >= 1; i--) {
		ChainArm(arrBossArm[i - 1], arrBossArm[i]);
	}
	ChainArm(arrBossArm[0], sx, sy);
	for (int i = 0; i < 3; i++) {
		ChainArm(arrBossArm[i + 1], arrBossArm[i]);
	}
	for (int i = 0; i < 4; i++) {
		arrBossArm[i]->inv = this->inv;
	}
}
#pragma endregion

void BossHand::ChainArm(BossArm* arm, BossHand* hand)
{
	float distance = 15;
	float tempX = CalculateDistance(arm->GetX(), hand->GetX(), distance);
	float tempY = CalculateDistance(arm->GetY(), hand->GetY(), distance);
	arm->SetDestination(tempX, tempY);

}

void BossHand::ChainArm(BossArm* arm, float sx, float sy)
{
	float tempX = CalculateDistance(arm->GetX(), sx, 0);
	float tempY = CalculateDistance(arm->GetY(), sy, 5);
	arm->SetDestination(tempX, tempY);
}

void BossHand::ChainArm(BossArm* arm, BossArm* armcheck)
{
	float distance = 15;
	float tempX = CalculateDistance(arm->GetX(), armcheck->GetX(), distance);
	float tempY = CalculateDistance(arm->GetY(), armcheck->GetY(), distance);
	arm->SetDestination(tempX, tempY);
}

float BossHand::CalculateDistance(float a, float b, float d)
{
	if (a - b > d)
	{
		return b + d;
	}
	if (a - b < -d)
	{
		return b - d;
	}
	return a;
}

ExplosiveBoss::ExplosiveBoss(int x, int y)
{
	this->x = x;
	this->y = y;

	animator->AddAnimation(22001);
	creationTime = GetTickCount();
}

void ExplosiveBoss::Update(float dt)
{
	if(GetTickCount() - creationTime > 500)
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void ExplosiveBoss::Render()
{
	animator->Draw(22001, x, y, false);
}