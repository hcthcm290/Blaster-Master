#include "Jason.h"
#include "Animator_Jason.h"
#include "Debug.h"
#include "PlayScene.h"
#include "Bullet_Jason.h"
#include "ColliableBrick.h"
#include "Sophia.h"

Jason::Jason() {
	animator = new Animator_Jason();
	state = State::_JASON_IDLE_;
	health = maxHealth;
}

Jason::Jason(int currentHealth, int x, int y, DynamicObject* sophia) {
	animator = new Animator_Jason();
	state = State::_JASON_JUMP_;
	this->x = x;
	this->y = y;
	this->sophia = sophia;
	health = currentHealth;
	switchDelay = GetTickCount64();
}

void Jason::Update(float dt)
{
	if (DInput::GetInstance()->KeyPress(DIK_LSHIFT) && CollisionSystem::CheckOverlap(this, sophia) 
		&& GetTickCount64() - switchDelay >= 1000) {
		dynamic_cast<Sophia*>(sophia)->Awake(health);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetPlayer(dynamic_cast<Sophia*>(sophia));
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
	}

	if (health <= 0) return;
	input->Update();

	UpdateActionRecord();

	vy -= jumpSpeed * (allowJump && attemptJump);
	vy += 150 * dt;

	vx = speed * horizontalMove;

	//reset position if Jason fell out
	if (DInput::GetInstance()->KeyPress(DIK_R)) {
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->HardReload();
	}

	//virtual damage trigger
	if (DInput::GetInstance()->KeyPress(DIK_Q)) {
		getDamage(1);
	}


	if (input->shoot) {
		Fire();
	}
}

void Jason::UpdateActionRecord() { //reset key input to catch newest keyboard
	//reset
	attemptJump = false;
	enviColX = enviColY = enemyColX = enemyColY = 0;
	damageTaken = 0;

	//update action record by input
	if (input->left && input->right) horizontalMove = horizontalMove; //hold old action
	else horizontalMove = input->left * (-1) + input->right * 1; //left or right

	if (input->up && input->down) verticalMove = verticalMove; //hold old action
	else verticalMove = input->up * (-1) + input->down * 1; //left or right

	attemptJump = input->jump;
}

void Jason::Render()
{
	flipX = (horizontalMove != 0 ? horizontalMove == -1 : flipX);
	SetNewState();

	//use color effect if they got damaged
	if (invulnerable != -1) {
		animator->Draw(state, x, y, flipX, 0, damageColor[invulnerable]);
		if (GetTickCount64() - damageEffectTimer >= 100) {
			damageEffectTimer = GetTickCount64();
			invulnerable = 1 - invulnerable; // 0 -> 1 -> 0 -> 1 -> ...
		}
	}
	else {
		animator->Draw(state, x, y, flipX);
	}
}

void Jason::SetNewState() {
	int newState = state;
	if (health <= 0) newState = State::_JASON_DIE_; //if no health, die anyway

	if (newState != State::_JASON_DIE_) { //if die, cannot perform any state
		switch (state) {
			case State::_JASON_CLIMB_: {
				allowJump = false;
				break;
			}
			case State::_JASON_CMOVE_: {
				allowJump = false;
				if (verticalMove == -1) {
					//y -= OFFSET_STAND_CRAWL; //offset to avoid overlapping
					newState = State::_JASON_IDLE_;
				}
				else if (horizontalMove == 0) {
					newState = State::_JASON_CRAWL_;
				}
				break;
			}
			case State::_JASON_CRAWL_: {
				allowJump = false;
				if (verticalMove == -1) {
					//y -= OFFSET_STAND_CRAWL; //offset to avoid overlapping
					newState = State::_JASON_IDLE_;
				}
				if (horizontalMove != 0) newState = State::_JASON_CMOVE_;
				break;
			}
			case State::_JASON_DIE_: {
				allowJump = false;
				vx = 0;
				vy = 0;
				break;
			}
			case State::_JASON_IDLE_: {
				allowJump = true;
				//collision with ladder
				if (verticalMove == 1) {
					//y += OFFSET_STAND_CRAWL;
					newState = State::_JASON_CRAWL_;
				}
				else if (attemptJump) {
					newState = State::_JASON_JUMP_;
				}
				else if (horizontalMove != 0) {
					newState = State::_JASON_WALK_;
				}
				break;
			}
			case State::_JASON_JUMP_: {
				allowJump = false;
				//switch to idle when fall into wall	
				if (enviColY < 0) {
					newState = State::_JASON_IDLE_;
				}
				break;
			}
			case State::_JASON_SWIM_: {
				allowJump = false;
				//swim swim swim swim
				break;
			}
			case State::_JASON_WALK_: {
				allowJump = true;
				if (verticalMove == 1) {
					newState = State::_JASON_CRAWL_;
				}
				else if (attemptJump) {
					newState = State::_JASON_JUMP_;
				}
				else if ( vx == 0 ) {
					newState = State::_JASON_IDLE_;
				}
				break;
			}
		}
	}

	bool stateChanged = (state != newState);
	FRECT stateColBox = GetCollisionBox();
	state = newState;
	FRECT newStateColBox = GetCollisionBox();

	y += ceil( ((stateColBox.bottom - stateColBox.top) - (newStateColBox.bottom - newStateColBox.top) ) / 2);

	//hot fix, will fix later
	/**
	if (stateChanged) {
		switch (state) {
		case State::_JASON_CRAWL_:
			y -= 5;
			break;
		case State::_JASON_DIE_:
			y -= 5;
			break;
		}
	}
	**/
}

FRECT Jason::GetCollisionBox() {
	float w = 10;
	float h = 200;
	switch (state) {
		case State::_JASON_CLIMB_	: w = 12; h = 16;	break;
		case State::_JASON_CMOVE_	: w = 16; h = 8;	break;
		case State::_JASON_CRAWL_	: w = 16; h = 8;	break;
		case State::_JASON_DIE_		: w = 16; h = 16;	break;
		case State::_JASON_IDLE_	: w = 10; h = 17;	break;
		case State::_JASON_JUMP_	: w = 10; h = 16;	break;
		case State::_JASON_SWIM_	: w = 17; h = 12;	break;
		case State::_JASON_WALK_	: w = 10; h = 16;	break;
	}
	return FRECT(x - w / 2, y - h / 2, x + w / 2, y + h / 2);
}
 
void Jason::OnCollisionEnter(CollisionEvent e) {
	/**
	//check sophia first
	if (dynamic_cast<Sophia*>(e.pGameObject)) {
		DebugOut(L"Sophia awake pls\n");
		if (DInput::GetInstance()->KeyPress(DIK_LSHIFT)) {
			dynamic_cast<Sophia*>(e.pGameObject)->Awake();
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetPlayer(dynamic_cast<Sophia*>(e.pGameObject));
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		}
	}
	*/
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) == NULL) return;

	//first this can only handle collision with environment
	enviColX = (e.nx != 0 ? e.nx : enviColX);
	enviColY = (e.ny != 0 ? e.ny : enviColY);

	//if (e.nx * vx < 0) vx = 0;
	if (e.ny < 0 ) vy = 0;
}

void Jason::getDamage(int damage) {
	DWORD thisTime = GetTickCount64();
	if (lastTakeDamage == 0  || thisTime > lastTakeDamage + invulnerableTime) {
		lastTakeDamage = thisTime;
		if (invulnerable == -1) {
			health -= damage;

			invulnerable = 1;
			DebugOut(L"Damage taken\n");
		}
		else invulnerable = -1;	
	}
}

void Jason::Fire() {
	DWORD thisTime = GetTickCount64();
	if (lastFire == 0 || thisTime > lastFire + recoilTime) {
		lastFire = thisTime;
		DynamicObject* newBullet = new Bullet_Jason(flipX ? -1 : 1, x, y);
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newBullet);
	}
}