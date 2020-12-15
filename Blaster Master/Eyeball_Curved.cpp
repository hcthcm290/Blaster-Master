#pragma once

#include "EyeBall_Curved.h"
#include "Debug.h"
#include "Explosion_Interior.h"
#include "PlayScene.h"
#include "ColliableBrick.h"
#include "Bullet_Eyeball.h"
#include "BigJason.h"

EyeballCurved::EyeballCurved() {
	animator = new Animator();
	animator->AddAnimation(State::_EYEBALL_);
	//get the player
	player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (player == NULL)
		DebugOut(L"ERROR: Player not found !!!\n");
}

void EyeballCurved::Update(float dt) {
	//DebugOut(L"Current pos: %f %f\n", x, y);
	switchTime -= dt;
	if (switchTime <= 0) { //its time to change state
		isResting = !isResting; //switch state
		switchTime = MAX_SWITCH_TIME;

		//0 - CHECK IF THE EYEBALL IS RESTING, IF REST STOP MOVING
		if (isResting) {
			//DebugOut(L"Stop !\n");
			vx0 = vy0 = 0;
			SetVelocity(0, 0); //no move
			return;
		}

		//DebugOut(L"Move !\n");
		//--> ensured: these code below is applied when isResting == false
		//1 - FIRE TO PLAYER
		Fire();

		//CHECK DIREFCTION --> go to CheckDirection() definition to continue reading
		CheckDirection();
		SetStartVelocity();
	}

	//if the eyeball is in turning time -> turn a angle of angleOffset
	if (switchTime <= startTurning && switchTime >= endTurning) {
		float alpha = Deg2Rad(angleOffset * dt);

		//calculate vx vy by turning function
		float new_vx0 = vx0 * cos(alpha) - vy0 * sin(alpha);
		float new_vy0 = vy0 * cos(alpha) + vx0 * sin(alpha);
	
		vx0 = new_vx0;
		vy0 = new_vy0;
	}
	
	//4 - MOVE IT	
	SetVelocity(vx0 * dt, vy0 * dt); //y-axis is flipped
}

void EyeballCurved::Render() {
	animator->Draw(State::_EYEBALL_, x, y, false);
}

FRECT EyeballCurved::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + 8);
}

void EyeballCurved::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<ColliableBrick*>(e.pGameObject)) {
		//CheckDirection();
		if (e.nx != 0) vx0 = -vx0; //horizontal
		else vy0 = -vy0; //vertical
	}
	if (dynamic_cast<BigJason*>(e.pGameObject)) {
		dynamic_cast<BigJason*>(e.pGameObject)->TakeDamage(20);
	}
}

void EyeballCurved::Die() {
	ExplosionInterior* explosion = new ExplosionInterior(x, y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explosion);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
}

void EyeballCurved::Fire() {
	//Fire a bullet to player
	Bullet_Eyeball* bullet = new Bullet_Eyeball(GetPosition().x, GetPosition().y);
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(bullet);
}

void EyeballCurved::CheckDirection() {
	//2 - GET 2 DIRECTION, THE FURTHER IS AT FIRST, THE ORTHER IS AT SECOND
	pair<DIRECTION, DIRECTION> newDirection = GetDirection();

	//3 - IF NEAREST DIRECTION != LAST NEAREST DIRECTION
	//UPDATE ANGLE
	if (newDirection.first != direction.first) {
		//DebugOut(L"Movepath Updated !\n");
		direction = newDirection;
		UpdateMovement();
		SetStartVelocity();
	}
}

pair<EyeballCurved::DIRECTION, EyeballCurved::DIRECTION> EyeballCurved::GetDirection() {
	pair<DIRECTION, DIRECTION> result;

	//calculate distance from eyeball to player
	dx = player->GetPosition().x - x;
	dy = player->GetPosition().y - y;

	//define which orientation is further
	if (abs(dx) > abs(dy)) result.first = (dx < 0 ? _LEFT : _RIGHT);
	else result.first = (dy < 0 ? _UP : _DOWN);
	
	//define the second direction which is away from player
	if (abs(dx) > abs(dy)) result.second = (dy < 0 ? _DOWN : _UP);
	else result.second = (dx < 0 ? _RIGHT : _LEFT);

	return result;
}

void EyeballCurved::UpdateMovement() {
	//the angle offset is scaled due to nearer direction (direction.second)
	float d;
	if (direction.second == _LEFT || direction.second == _RIGHT) //horizontal
		d = abs(dx);
	else //vertical
		d = abs(dy);

	if (d == 0) //avoid zero-divisor 
		d = 1;

	//Change speed
	speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * (MAX_L / d);
	speed = min(MAX_SPEED, speed);
	speed = max(MIN_SPEED, speed);

	// L / d == MAX_SWITCH_TIME / MAX_L
	float L = MAX_SWITCH_TIME * MAX_L * ADJUSTMENT_VARIABLE / d;

	//set time
	endTurning = 1 - L / 2;
	startTurning = endTurning + L;

	//set angleOffset;
	angleOffset = 180 / L;
	angleOffset = min(MAX_ANGLE_OFFSET, angleOffset);
	angleOffset = max(MIN_ANGLE_OFFSET, angleOffset);

	//switch angleOffset in special cases
	//because y-axis is flipped so everything is fukin fliped too
	if (direction != pair<DIRECTION,DIRECTION>{_DOWN, _LEFT} 
	&& direction != pair<DIRECTION, DIRECTION>{_LEFT, _UP}
	&& direction != pair<DIRECTION, DIRECTION>{_UP, _RIGHT}
	&& direction != pair<DIRECTION, DIRECTION>{_RIGHT, _DOWN})
		angleOffset = -angleOffset;
}

void EyeballCurved::SetStartVelocity() {
	//Set start velocity	
	if (direction.first == _LEFT || direction.first == _RIGHT) {	//horizontal
		vx0 = (direction.first == _LEFT ? -1 : 1) * speed;
		vy0 = 0;
	}
	else {	//vertical
		vx0 = 0;
		vy0 = (direction.first == _UP ? -1 : 1) * speed;
	}
	//DebugOut(L"vx0: %f vy0: %f \n", vx0, vy0);
}