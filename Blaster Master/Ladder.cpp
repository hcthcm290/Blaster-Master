#include "Ladder.h"
#include "StateConstant.h"
#include "CollisionSystem.h"
#include "Debug.h"

Ladder::Ladder(int h) {
	animator = new Animator();
	animator->AddAnimation(State::_LADDER_);
	this->height = h;
}

void Ladder::Update(float dt) {
	if (top == NULL) {
		top = y - 8;
		bottom = y + height * 16 - 9; //avoid flicking between body and tail
	}

	if (jason != NULL && !CollisionSystem::CheckOverlap(this, jason)) {
		jason = NULL;
	}

	//if jason is jumping, ignore
	if (jason != NULL && jason->GetState() == State::_JASON_JUMP_) {
		return;
	}

	if (jason != NULL) {
		FRECT jasonColBox = jason->GetCollisionBox();
		if (jasonColBox.top <= top) {
			jason->onLadderState = Head;
		}
		else if (jasonColBox.bottom >= bottom) {
			jason->onLadderState = Tail;
		}
		else {
			float jasonY = jason->GetPosition().y;
			jason->SetPosition(this->x, jasonY);
			jason->onLadderState = Body;
		}
	}
}

void Ladder::Render() {
	if (jason != NULL) {
		switch (jason->onLadderState) {
		case Tail: 
			DebugOut(L"Tail\n");
			break; 
		case Head:
			DebugOut(L"Head\n");
			break;
		case Body:
			DebugOut(L"Body\n");
			break;
		case Null:
			DebugOut(L"Tail\n");
			break;
		}
	}

	for (int i = 0; i < height; i++) {
		animator->Draw(State::_LADDER_, x, y + i * 16, false);
	}
}

FRECT Ladder::GetCollisionBox() {
	return FRECT(x - 8, y - 8, x + 8, y + height * 16 - 8);
}

void Ladder::OnCollisionEnter(CollisionEvent e) {
	if (dynamic_cast<Jason*>(e.pGameObject) != NULL) {
		jason = dynamic_cast<Jason*>(e.pGameObject);
	}
}