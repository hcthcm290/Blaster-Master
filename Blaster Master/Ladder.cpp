#include "Ladder.h"
#include "StateConstant.h"
#include "CollisionSystem.h"
#include "Debug.h"
#include "PInput.h"

Ladder::Ladder(int h) {
	animator = new Animator();
	animator->AddAnimation(State::_LADDER_);
	this->height = h;
	isLeft = false;
	isRight = false;
}

void Ladder::Update(float dt) {
	
}

void Ladder::Render() {
	/*for (int i = 0; i < height; i++) {
		animator->Draw(State::_LADDER_, x, y + i * 16, false);
	}*/
}

FRECT Ladder::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + height * 16 - 8);
}

void Ladder::OnCollisionEnter(CollisionEvent e) {
	
}

LadderPos Ladder::CheckLadderPos(int jasonState, FRECT jasonColBox) {
	if (top == NULL) {
		top = y - 8;
		bottom = y + height * 16 - 9; //avoid flicking between body and tail
	}
	
	LadderPos result = LadderPos::Body;

	//PHASE 1
	if (jasonState == State::_JASON_JUMP_ || PInput::KeyPressed(JUMP)) {
		result = LadderPos::Null;
	}
	else if (jasonColBox.bottom >= bottom) {
		result = LadderPos::Bottom;
	}
	else if (jasonColBox.top <= top) {
		result = LadderPos::Top;
	}

	//PHASE 2
	if (result == LadderPos::Top) {
		if (PInput::OneOfThoseIsPressed(NULL, NULL, true, true, NULL, true, true, NULL)) {
			result = LadderPos::Null;
			isOut = true;
		}
		else if (PInput::KeyDown(DOWN)) {
			result = LadderPos::Body;
			isOut = false;
		}
	}
	else if (result == LadderPos::Bottom) {
		if (PInput::OneOfThoseIsPressed(NULL, true, true, true, NULL, true, true, NULL)) {
			result = LadderPos::Null;
			isOut = true;
		}
		else if (PInput::KeyDown(UP)) {
			result = LadderPos::Body;
			isOut = false;
		}
	}

	if (PInput::KeyDown(LEFT))
	{
		isLeft = true;
	}
	else
	{
		if (PInput::KeyDown(RIGHT))
			isRight = true;
	}

	if (isOut)
	{
		if (isRight && isLeft)
		{
			isRight = isLeft = false;
			isOut = false;
		}
		else
		{
			result = LadderPos::Null;
		}
	}

	
	//switch (result) {
	//case LadderPos::Null:
	//	DebugOut(L"Null\n");
	//	break;
	//case LadderPos::Top:
	//	DebugOut(L"Top\n");
	//	break;
	//case LadderPos::Bottom:
	//	DebugOut(L"Bottom\n");
	//	break;
	//case LadderPos::Body:
	//	DebugOut(L"Body\n");
	//	break;
	//}
	
	return result;
}