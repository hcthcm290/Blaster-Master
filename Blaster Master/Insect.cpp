#include "Insect.h"
#include "StateConstant.h"
#include "Debug.h"
#include "DynamicObject.h"

Insect::Insect() {
	animator = new Animator();
	animator->AddAnimation(State::_INSECT_);
}

void Insect::Update(float dt)
{
	//reset some variable
	ColX = 0;
	ColY = 0;

	//update vx vy
	vx = 50 * (flipX ? -1 : 1);
	vy += 100 * dt;

	//update vx vy up to state
	switch ( state )
	{
	case GoUp:
		if (vy >= 0) vy -= 100;
		break;
	case HoldHeight:
		vy = 5;
		break;
	case FallDown:
		vy += 50;
		break;
	}
}

void Insect::Render()
{
	SetNextState();
	animator->Draw(State::_INSECT_, x, y, flipX);
}

FRECT Insect::GetCollisionBox() {
	return FRECT(x - 9, y - 9, x + 9, y + 9);
}

void Insect::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<DynamicObject*>(e.pGameObject)) return;

	ColX = (e.nx != 0 ? e.nx : ColX);
	ColY = (e.ny != 0 ? e.ny : ColY);
	//DebugOut(L"OUCH !!\n");

	if (ColX != 0) 
		flipX = !flipX;
}

void Insect::SetNextState() {
	InsectState newState = state;
	DWORD thisTime = GetTickCount64();
	switch (state)
	{
	case GoUp:
		if (vy < 0 && ColY > 0) newState = FallDown;
		else if (vy > 0) {
			lastTime = thisTime;
			newState = HoldHeight;
		}
		break;
	case HoldHeight:
		if (thisTime >= lastTime + 2000 || ColY < 0)
			newState = GoUp;
		break;
	case FallDown:
		if (ColY < 0)
			newState = GoUp;
		break;
	}

	//debug
	/**
	if (newState != state) {
		switch (newState) {
		case GoUp:
			DebugOut(L"GoUp\n");
			break;
		case HoldHeight:
			DebugOut(L"HoldHeight\n");
			break;
		case FallDown:
			DebugOut(L"FallDown\n");
			break;
		}
	}
	*/

	state = newState;
}