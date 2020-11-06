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
	input = new PlayerInput();
}

void Jason::Update(float dt)
{
	input->Update();

	UpdateActionRecord();

	vx = speed * horizontalMove;
	vy -= speed * (state != State::_JASON_JUMP_ && attemptJump);
	vy += 100 * dt;

	if (DInput::GetInstance()->KeyPress(DIK_R)) {
		x = 1120;
		y = 1120;
	}
	DebugOut(_wcsdup(StateToString().c_str()));
}

void Jason::UpdateActionRecord() { //reset key input to catch newest keyboard
	//reset
	attemptJump = false;
	enviColX = enviColY = enemyColX = enemyColY = 0;

	//update action record by input
	if (input->left && input->right) horizontalMove = horizontalMove; //hold old action
	else horizontalMove = input->left * (-1) + input->right * 1; //left or right

	if (input->up && input->down) verticalMove = verticalMove; //hold old action
	else verticalMove = input->up * (-1) + input->down * 1; //left or right

	attemptJump = input->jump;
	outOfHealth = (health <= 0);
}

void Jason::Render()
{
	flipX = (horizontalMove != 0 ? horizontalMove == -1 : flipX);
	state = SetNewState();
	animator->Draw(state, x, y, flipX);
}

int Jason::SetNewState() {
	if (outOfHealth) return State::_JASON_DIE_;
	switch (state) {
		case State::_JASON_CLIMB_: {

			break;
		}
		case State::_JASON_CMOVE_: {
			if (verticalMove == -1) {
				y -= OFFSET_STAND_CRAWL; //offset to avoid overlapping
				return State::_JASON_IDLE_;
			}
			else if (horizontalMove == 0) {
				return State::_JASON_CRAWL_;
			}
			break;	
		}
		case State::_JASON_CRAWL_: {
			if (verticalMove == -1) {
				y -= OFFSET_STAND_CRAWL; //offset to avoid overlapping
				return State::_JASON_IDLE_;
			}
			if (horizontalMove != 0) return State::_JASON_CMOVE_;
			break;
		}
		case State::_JASON_DIE_: {

			break;
		}
		case State::_JASON_IDLE_: {
			//collision with ladder
			if (verticalMove == 1) {
				//y += OFFSET_STAND_CRAWL;
				return State::_JASON_CRAWL_;
			}
			else if (attemptJump) {
				return State::_JASON_JUMP_;
			}
			else if (horizontalMove != 0) {
				return State::_JASON_WALK_;
			}
			break;
		}
		case State::_JASON_JUMP_: {
			//switch to idle when fall  wall	
			if ( vy == 0 ) {
				return State::_JASON_IDLE_;
			}
			break;
		}
		case State::_JASON_SWIM_: {
			//swim swim swim swim
			break;
		}
		case State::_JASON_WALK_: {
			if (verticalMove == 1) {
				return State::_JASON_CRAWL_;
			}
			else if (attemptJump) {
				return State::_JASON_JUMP_;
			}
			else if (vx == 0 || enviColX * vx < 0) {
				return State::_JASON_IDLE_;
			}
			break;
		}			
	}
	return state;
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
	//first this can only handle collision with environment
	enviColX = (enviColX != 0 ? e.nx : enviColX);
	enviColY = (enviColY != 0 ? e.ny : enviColY);

	if (e.nx * vx < 0) vx = 0; 
	if (e.ny != 0 ) vy = 0;
}

wstring Jason::StateToString() {
	wstring r;
	switch (state) {
		case State::_JASON_CLIMB_:	r = L"Climb\n";	break;
		case State::_JASON_CMOVE_:	r = L"Cmove\n";	break;
		case State::_JASON_CRAWL_:	r = L"Crawl\n";	break;
		case State::_JASON_DIE_:	r = L"Die\n";	break;
		case State::_JASON_IDLE_:	r = L"Idle\n";	break;
		case State::_JASON_JUMP_:	r = L"Jump\n";	break;
		case State::_JASON_SWIM_:	r = L"Swim\n";	break;
		case State::_JASON_WALK_:	r = L"Walk\n";	break;
	}
	return r;
}
