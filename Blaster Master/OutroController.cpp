#include "OutroController.h"
#include "Game.h"
#include "Debug.h"
#include "Camera.h"

OutroController::OutroController()
{
	this->RedSceneAnimator.AddAnimation(this->RedSceneAnimationID);
}

void OutroController::Update(float dt)
{
	this->CountTime += dt;

	if (CountTime > this->mountainDelayFalling)
	{
		mountainPosition.y += mountainFallingSpeed * dt;
	}

	if (CountTime > this->cameraDelayShaking && CountTime < this->cameraDelayShaking + this->cameraShakingDuration)
	{
		this->cameraShakingCountTime += dt;
		if (this->cameraShakingCountTime >= this->cameraShakingInterval)
		{
			if (this->cameraMovingDirection.y == 0)
			{
				this->cameraMovingDirection.y = -1;
			}
			if (this->cameraMovingDirection.y < 0)
			{
				auto cameraPosition = Camera::GetInstance()->GetPosition();
				Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y - 2);
				cameraShakingCountTime = 0;
			}
			else if(this->cameraMovingDirection.y > 0)
			{
				auto cameraPosition = Camera::GetInstance()->GetPosition();
				Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y + 2);
				cameraShakingCountTime = 0;
			}

			this->cameraMovingDirection.y = -this->cameraMovingDirection.y;
		}
	}
}

void OutroController::Render()
{
	if (this->CountTime < this->mountainLifetime + mountainDelayFalling)
	{
		CSprites::GetInstance()->Get(this->mountainSpriteID)->Draw(this->mountainPosition.x, this->mountainPosition.y);
	}
	if (this->CountTime < this->RedSceneLifetime)
	{
		this->RedSceneAnimator.Draw(this->RedSceneAnimationID, this->RedAnimationPosition.x, this->RedAnimationPosition.y, false);
		DebugOut(L"current frame: %d\n", this->RedSceneAnimator.curFrame);
	}
}
