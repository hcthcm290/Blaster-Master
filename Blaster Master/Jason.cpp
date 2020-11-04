#include "Jason.h"
#include "Animator_Jason.h"

Jason::Jason() {
	animator = new Animator_Jason();
	state = State::_JASON_IDLE_;
	transform.height = IDLE_HEIGHT;
}

void Jason::Update(DWORD dt)
{
	MakeCrouch();
	MakeMove();
	MakeJump();
	GravityEffect();
	x += vx;
	y += vy;
	
	//virtual collision
	if (y + transform.height > initialY) {
		y = initialY - transform.height; 
		vy = 0;
		if (state != State::_JASON_WALK_ && state!=State::_JASON_CRAWL_ && state!=State::_JASON_CMOVE_)
			state = State::_JASON_IDLE_;
	}
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
		y += IDLE_HEIGHT - CRAW_HEIGHT;
		transform.height = CRAW_HEIGHT;
	}
	else if (state == State::_JASON_CRAWL_ && up) {
		state = State::_JASON_IDLE_;
		//y -= IDLE_HEIGHT - CRAW_HEIGHT
		transform.height = IDLE_HEIGHT;
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


void Jason::MakeJump() {
	bool jump = DInput::GetInstance()->KeyDown(DIK_X);
	if ((state==State::_JASON_IDLE_ || state==State::_JASON_WALK_)&& jump) 
	{
		vy = -30;
		state = State::_JASON_JUMP_;
	}
}

void Jason::GravityEffect() {
	vy += Environment::GRAVITY;
}