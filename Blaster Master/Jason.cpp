#include "Jason.h"
#include "Animator_Jason.h"
#include "Debug.h"
#include "PlayScene.h"
#include "Bullet_Jason.h"
#include "ColliableBrick.h"
#include "Sophia.h"
#include "PInput.h"
#include "TheEye.h"
#include "SoundManager.h"

Jason::Jason() {

	Jason* prevJason = TheEye::GetInstance()->GetJason();
	if (prevJason == NULL)
	{
		bulletPower = 0;
	}
	else
	{
		bulletPower = prevJason->bulletPower;
	}

	animator = new Animator_Jason();
	currentState = State::_JASON_IDLE_;
	HP = 40;
	maxHP = 40;

	TheEye::GetInstance()->SetJason(this);
}

Jason::Jason(int currentHealth, int x, int y, DynamicObject* sophia) {
	animator = new Animator_Jason();
	currentState = State::_JASON_JUMP_;
	this->x = x;
	this->y = y;
	this->sophia = sophia;
	HP = currentHealth;
	maxHP = currentHealth;
	switchDelay = GetTickCount64();
	
	SoundManager::GetInstance()->PlaySoundW("swapSophiaAndJason.wav");

	Jason* prevJason = TheEye::GetInstance()->GetJason();
	if (prevJason == NULL)
	{
		bulletPower = 0;
	}
	else
	{
		bulletPower = prevJason->bulletPower;
	}

	TheEye::GetInstance()->SetJason(this);
}

void Jason::Update(float dt)
{
	#pragma region SOPHIA TRANSITION
	/** SOPHIA TRANSITION */
	if (currentState == State::_JASON_IDLE_ && PInput::KeyDown(SHIFT) 
		&& CollisionSystem::CheckOverlap(this, sophia)
		&& GetTickCount64() - switchDelay >= 1000) {

		vy = -jumpSpeed*1.25;
		attemptJump = true;
		switchEffectDuration = 0.26f;
		dynamic_cast<Sophia*>(sophia)->Awake(HP);
	}

	if (switchEffectDuration > 0) {
		switchEffectDuration -= dt;
		vy += 150*dt;
		if (switchEffectDuration <= 0) {
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetPlayer(dynamic_cast<Sophia*>(sophia));
			dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
		}
		return;
	}
	#pragma endregion

	if (HP <= 0)
	{
		vx = 0;
		vy = 0;
		return;
	}

	UpdateActionRecord();

	#pragma region LADDER CHECK
	//LADDER CHECK
	if (ladder != NULL) {
		//if ladder is no more collide with jason, remove it
		if (CollisionSystem::CheckOverlap(ladder, this)) {
			ladderPos = ladder->CheckLadderPos(currentState, GetCollisionBox());
		}
		else {
			ladder = NULL;
		}
	}
	else ladderPos = LadderPos::Null;
	//-------------------------------//
	#pragma endregion

	#pragma region Update state vx vy up to LADDER
	//UPDATE VX VY
	if (ladderPos == LadderPos::Null) { //FREE MOVEMENT
		if (GetState() == State::_JASON_CLIMB_)
			SetState(State::_JASON_IDLE_);

		vx = horizontalMove * speed;
		vy -= jumpSpeed * (allowJump && attemptJump);
		vy += 150 * dt * (ladder == NULL 
							|| !CollisionSystem::CheckOverlap(ladder, this)
							|| attemptJump == false);
	}
	else if (ladderPos == LadderPos::Top) { //TOP OF LADDER
		if (GetState() == State::_JASON_CLIMB_)
			SetState(State::_JASON_IDLE_);
		vx = 0;
		vy = 0;
	}
	else if (ladderPos == LadderPos::Bottom) { //BOTTOM OF LADDER
		if (GetState() == State::_JASON_CLIMB_)
			SetState(State::_JASON_IDLE_);
		vx = 0;
		vy = 0;
	}
	else if (ladderPos == LadderPos::Body) { //BODY OF LADDER
		SetState(State::_JASON_CLIMB_);
		x = ladder->GetPosition().x;
		vx = 0;
		if (verticalMove != 0) {
			vy = verticalMove * speed / 5;
			flipCountDown -= dt;
			if (flipCountDown <= 0) {
				flipX = ~flipX;
				flipCountDown = 0.25f;
			}
		}
		else vy = 0;
	}
	//-------------------------------//
	#pragma endregion

	/**
	if (currentState!=State::_JASON_JUMP_ && onLaddercurrentState == Head) {
		if (verticalMove > 0) vy = verticalMove * speed / 5 - jumpSpeed * (allowJump && attemptJump);
		else {
			vy = verticalMove * speed / 5 - jumpSpeed * (allowJump && attemptJump);
			vx = horizontalMove * speed;	
		}
	}
	else if (currentState != State::_JASON_JUMP_ && onLaddercurrentState == Body) {
		currentState = State::_JASON_CLIMB_;
		vx = 0;
		if (verticalMove != 0) {
			vy = verticalMove * speed / 5 - jumpSpeed * (allowJump && attemptJump);
			flipCountDown -= dt;
			if (flipCountDown <= 0) {
				flipX = ~flipX;
				flipCountDown = 0.25f;
			}
		}
		else vy = 0;
	}
	else if (currentState != State::_JASON_JUMP_ && onLaddercurrentState == Tail) {
		if (verticalMove < 0) vy = verticalMove * speed / 5;
		else {
			//Jason can stop climbing and fall, so no (vy = 0)
			vy = verticalMove * speed / 5 - jumpSpeed * (allowJump && attemptJump);
			vx = horizontalMove * speed;
		}
	}
	else {
		vx = horizontalMove * speed;
		vy -= jumpSpeed * (allowJump && attemptJump);
		vy += 150 * dt;
	}
	**/

	if ( PInput::KeyDown(SHOOT) && allowShoot ) {
		Fire();
	}
}

void Jason::UpdateActionRecord() { //reset key input to catch newest keyboard
	//reset
	attemptJump = false;
	enviColX = enviColY = enemyColX = enemyColY = 0;

	//update action record by input
	if ( PInput::KeyPressed(LEFT) && PInput::KeyPressed(RIGHT) ) horizontalMove = horizontalMove; //hold old action
	else horizontalMove = PInput::KeyPressed(LEFT) * (-1) + PInput::KeyPressed(RIGHT) * 1; //left or right

	if ( PInput::KeyPressed(UP) && PInput::KeyPressed(DOWN) ) verticalMove = verticalMove; //hold old action
	else verticalMove = PInput::KeyPressed(UP) * (-1) + PInput::KeyPressed(DOWN) * 1; //left or right

	attemptJump = PInput::KeyDown(JUMP);
	if (attemptJump)
	{
		SoundManager::GetInstance()->PlaySoundW("JasonJump.wav");
	}
}

void Jason::Render()
{
	if (animator == NULL)
	{
		animator = new Animator_Jason;
	}

	flipX = (horizontalMove != 0 ? horizontalMove == -1 : flipX);
	SetNewState();

	//use color effect if they got damaged
	if (invulnerable != -1) {
		animator->Draw(currentState, x, y, flipX, 0, damageColor[invulnerable]);
		if (GetTickCount64() - damageEffectTimer >= 100) {
			if (GetTickCount64() > lastTakeDamage + invulnerableTime)
			{
				invulnerable = -1;
			}
			else
			{
				damageEffectTimer = GetTickCount64();
				switch (invulnerable)
				{
				case 3: invulnerable = 2; break;
				case 2: invulnerable = 1; break;
				case 1: invulnerable = 0; break;
				case 0: invulnerable = 3; break;
				}
			}
		}
	}
	else {
		animator->Draw(currentState, x, y, flipX);
	}
}

void Jason::SetNewState() {
	int newState = currentState;
	if (HP <= 0) newState = State::_JASON_DIE_; //if no HP, die anyway

	if (newState != State::_JASON_DIE_) { //if die, cannot perform any currentState
		switch (currentState) {
			case State::_JASON_CLIMB_: {
				allowJump = true;
				allowShoot = false;
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

				if (attemptJump) {
					newState = State::_JASON_JUMP_;
				}
				else if (horizontalMove != 0) {
					newState = State::_JASON_WALK_;
				}
				else if (PInput::KeyDown(DOWN)) {
					newState = State::_JASON_CRAWL_;
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

	bool currentStateChanged = (currentState != newState);
	FRECT currentStateColBox = GetCollisionBox();
	currentState = newState;
	FRECT newStateColBox = GetCollisionBox();

	y += ceil( ((currentStateColBox.bottom - currentStateColBox.top) - (newStateColBox.bottom - newStateColBox.top) ) / 2);
}

FRECT Jason::GetCollisionBox() {
	float w = 10;
	float h = 200;
	switch (currentState) {
		case State::_JASON_CLIMB_	: w = 12; h = 16;	break;
		case State::_JASON_CMOVE_	: w = 16; h = 8;	break;
		case State::_JASON_CRAWL_	: w = 16; h = 8;	break;
		case State::_JASON_DIE_		: w = 16; h = 16;	break;
		case State::_JASON_IDLE_	: w = 10; h = 15;	break;
		case State::_JASON_JUMP_	: w = 10; h = 16;	break;
		case State::_JASON_SWIM_	: w = 17; h = 12;	break;
		case State::_JASON_WALK_	: w = 10; h = 15;	break;
	}
	return FRECT(x - w / 2, y - h / 2, x + w / 2, y + h / 2);
}
 
void Jason::OnCollisionEnter(CollisionEvent e) {
	//Check if collisiion with ladder
	if (dynamic_cast<Ladder*>(e.pGameObject) != NULL) {
		ladder = dynamic_cast<Ladder*>(e.pGameObject);
		DebugOut(L"Ouch\n");
		return;
	}

	if (dynamic_cast<ColliableBrick*>(e.pGameObject) == NULL) return;

	//first this can only handle collision with environment
	enviColX = (e.nx != 0 ? e.nx : enviColX);
	enviColY = (e.ny != 0 ? e.ny : enviColY);

	//if (e.nx * vx < 0) vx = 0;
	if (e.ny < 0 ) vy = 0;
}

float Jason::GetEnterGateSpeed()
{
	return 40;
}

void Jason::TakeDamage(int dmg) {
	DWORD thisTime = GetTickCount64();
	if (lastTakeDamage == 0  || thisTime > lastTakeDamage + invulnerableTime) {
		SoundManager::GetInstance()->PlaySoundW("JasonGotHit_Outside.wav");
		lastTakeDamage = thisTime;
		if (invulnerable == -1) {
			HP -= dmg;
			if (HP < 0)
			{
				HP = 0;
				SoundManager::GetInstance()->PlaySoundW("JasonDie.wav");
			}
			invulnerable = 3;
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