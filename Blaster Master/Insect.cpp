#include "Insect.h"
#include "StateConstant.h"
#include "Debug.h"
#include "DynamicObject.h"
#include "ColliableBrick.h"
#include "PlayScene.h"
#include "CollisionSystem.h"
#include "Sound.h"

Insect::Insect() {
	//set HP
	HP = 30;

	animator = new Animator();
	animator->AddAnimation(State::_INSECT_);
}

void Insect::Update(float dt)
{
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(5);
	}
	//reset some variable
	ColX = 0;
	ColY = 0;

	//update vx vy
	vx = speed * (flipX ? 1 : -1);
	vy += 300 * dt;

	soundCount += dt;
	//update vx vy up to state
	switch ( state )
	{
	case Forceburst:
		vy = speed * (-6);
		break;
	case GoUp:
		//do nothing: Trung Võ
			//Noooo: Trung Nguyễn
			//do Sound
		if (soundCount >= 1)
		{
			Sound::getInstance()->play("Insect",false,1);
			soundCount = 0;
		}
		break;
	case HoldHeight:
		vy = abs(speed / 3);
		break;
	case FallDown:
		vy *= 1.25; //fall down faster
		vx /= 3;
		break;
	}

	//DebugOut(L"%f\n", vy);
}

void Insect::Render()
{
	SetNextState();
	if (inv != -1) {
		animator->Draw(State::_INSECT_, x, y, flipX, 0, Color[inv]);
		if (GetTickCount64() - last_blink >= 50) {
			if (GetTickCount64() > startTakeDamage + 150)
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
		animator->Draw(State::_INSECT_, x, y, flipX);
	}
}

FRECT Insect::GetCollisionBox() {
	return FRECT(x - 9, y - 9, x + 9, y + 9);
}

void Insect::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) == NULL) return;

	ColX = (e.nx != 0 ? e.nx : ColX);
	ColY = (e.ny != 0 ? e.ny : ColY);
	//DebugOut(L"OUCH !!\n");
}

void Insect::SetNextState() {
	if (ColX != 0) flipX = !flipX;

	InsectState newState = state;
	DWORD thisTime = GetTickCount64();
	switch (state)
	{
		case Forceburst: {
			if (ColY > 0) {
				newState = FallDown;
				//DebugOut(L"FallDown\n");
			}
			else {
				newState = GoUp;
				break;
			}
			//DebugOut(L"GoUp\n");
		}
		case GoUp: {
			if (ColY > 0) {
				newState = FallDown;
				//DebugOut(L"FallDown\n");
			}
			else if (vy >= 0) {
				lastHoldHeightTime = thisTime;
				newState = HoldHeight;
				//DebugOut(L"HoldHeight\n");
			}
			break;
		}
		case HoldHeight: {
			if (thisTime >= lastHoldHeightTime + 1000 || ColY < 0) {
				newState = Forceburst;
				//DebugOut(L"ForeBurst\n");
			}
				
			break;
		}
		case FallDown: {
			if (ColY < 0) {
				newState = Forceburst;
				//DebugOut(L"ForeBurst\n");
			}
			break;
		}
	}
	
	state = newState;
}