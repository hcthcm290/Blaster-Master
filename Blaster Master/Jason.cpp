#include "Jason.h"
#include "Animator_Jason.h"
#include "Debug.h"

Jason::Jason() {
	animator = new Animator_Jason();
	animator->AddAnimation(State::_JASON_IDLE_);
	animator->AddAnimation(State::_JASON_WALK_);
	animator->AddAnimation(State::_JASON_JUMP_);
	animator->AddAnimation(State::_JASON_CLIMB_);
	animator->AddAnimation(State::_JASON_CRAWL_);
	animator->AddAnimation(State::_JASON_CMOVE_);
	animator->AddAnimation(State::_JASON_DIE_);
	state = State::_JASON_DIE_;
}

void Jason::Update(float dt)
{
	MakeCrouch();
	MakeMove();
	MakeJump(dt);
	GravityEffect();
	x += vx*dt;
	y += vy*dt;
	
	//virtual collision
	/**
	if (y + transform.height > initialY) {
		y = initialY - transform.height; 
		vy = 0;
		if (state != State::_JASON_WALK_ && state!=State::_JASON_CRAWL_ && state!=State::_JASON_CMOVE_)
			state = State::_JASON_IDLE_;
	}
	*/
}

void Jason::Render()
{
	animator->Draw(state, x, y, flipX);
}

void Jason::MakeCrouch() {
	bool up = DInput::GetInstance()->KeyDown(DIK_UP);
	bool down = DInput::GetInstance()->KeyDown(DIK_DOWN);
	if (state == State::_JASON_IDLE_ && down) {
		state = State::_JASON_CRAWL_;
		y += 4;
	}
	else if (state == State::_JASON_CRAWL_ && up) {
		state = State::_JASON_IDLE_;
	}
}

void Jason::MakeMove() {
	bool left = DInput::GetInstance()->KeyDown(DIK_LEFT);
	bool right = DInput::GetInstance()->KeyDown(DIK_RIGHT);

	if (left || right) {
		flipX = (left && right ? flipX : left);
		vx = (flipX ? -1 : 1) * speed;
		if (state == State::_JASON_IDLE_) state = State::_JASON_WALK_;
		else if (state == State::_JASON_CRAWL_) state = State::_JASON_CMOVE_;
	}
	else {
		vx = 0;
		if (state == State::_JASON_WALK_) state = State::_JASON_IDLE_;
		else if (state == State::_JASON_CMOVE_) state = State::_JASON_CRAWL_;
	}
}


void Jason::MakeJump(float dt) {
	bool jump = DInput::GetInstance()->KeyDown(DIK_X);
	if ((state==State::_JASON_IDLE_ || state==State::_JASON_WALK_)&& jump) 
	{
		vy = -30;
		state = State::_JASON_JUMP_;
	}
}

void Jason::GravityEffect() {
	//vy += Environment::GRAVITY;
}

FRECT Jason::GetCollisionBox()
{
	FRECT colRect;
	colRect.left = x - 10 / 2;
	colRect.right = x + 10 / 2;
	colRect.top = y - 17 / 2;
	colRect.bottom = y + 17 / 2;

	return colRect;
}