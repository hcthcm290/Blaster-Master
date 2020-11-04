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
	state = State::_JASON_IDLE_;
}

void Jason::Update(float dt)
{
	vy += 300 * dt;

	MakeCrouch();
	MakeMove();
	MakeJump();
	//x += vx*dt;
	//y += vy*dt;
	
	//virtual collision
	/**
	if (y + transform.height > initialY) {
		y = initialY - transform.height; 
		vy = 0;
		if (state != State::_JASON_WALK_ && state!=State::_JASON_CRAWL_ && state!=State::_JASON_CMOVE_)
			state = State::_JASON_IDLE_;
	}
	*/
	DebugOut(_wcsdup(StateToString().c_str()));
}

void Jason::Render()
{
	animator->Draw(state, x, y, flipX);
}

void Jason::MakeCrouch() {
	bool up = DInput::GetInstance()->KeyDown(DIK_UP);
	bool down = DInput::GetInstance()->KeyDown(DIK_DOWN);
	if (state == State::_JASON_IDLE_ && down) {
		y += 4;
		state = State::_JASON_CRAWL_;
	}
	else if (state == State::_JASON_CRAWL_ && up) {
		y -= 4;
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


void Jason::MakeJump() {
	bool jump = DInput::GetInstance()->KeyDown(DIK_X);
	if ((state==State::_JASON_IDLE_ || state==State::_JASON_WALK_)&& jump) 
	{
		vy = -speed;
		state = State::_JASON_JUMP_;
	}
}

FRECT Jason::GetCollisionBox() {
	float w = 10;
	float h = 17;
	switch (state) {
		case State::_JASON_CLIMB_	: w = 12; h = 16; break;
		case State::_JASON_CMOVE_	: w = 16; h = 10; break;
		case State::_JASON_CRAWL_	: w = 16; h = 10; break;
		case State::_JASON_DIE_		: w = 16; h = 16; break;
		case State::_JASON_IDLE_	: w = 10; h = 17; break;
		case State::_JASON_JUMP_	: w = 10; h = 16; break;
		case State::_JASON_SWIM_	: w = 17; h = 12; break;
		case State::_JASON_WALK_	: w = 10; h = 17; break;
	}
	return FRECT(x - w / 2, y - h / 2, x + w / 2, y + h / 2);
}

void Jason::OnCollisionEnter(CollisionEvent e) {
	//DebugOut(L"OUCH\n");
	state = State::_JASON_IDLE_;
}

wstring Jason::StateToString() {
	wstring r;
	switch (state) {
	case State::_JASON_CLIMB_: r = L"Climb\n"; break;
	case State::_JASON_CMOVE_: r = L"Cmove\n"; break;
	case State::_JASON_CRAWL_: r = L"Crawl\n"; break;
	case State::_JASON_DIE_: r = L"Die\n"; break;
	case State::_JASON_IDLE_: r = L"Idle\n"; break;
	case State::_JASON_JUMP_: r = L"Jump\n"; break;
	case State::_JASON_SWIM_: r = L"Swim\n"; break;
	case State::_JASON_WALK_: r = L"Walk\n"; break;
	}
	return r;
}
