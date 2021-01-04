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

	#pragma region Moutain falling

	if (CountTime > this->mountainDelayFalling)
	{
		mountainPosition.y += mountainFallingSpeed * dt;
	}

	#pragma endregion
	
	#pragma region CameraShaking

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
			else if (this->cameraMovingDirection.y > 0)
			{
				auto cameraPosition = Camera::GetInstance()->GetPosition();
				Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y + 2);
				cameraShakingCountTime = 0;
			}
			this->cameraMovingDirection.y = -this->cameraMovingDirection.y;
		}
	}

	#pragma endregion

	#pragma region CameraSliding

	if (CountTime > this->cameraDelayShaking + this->cameraShakingDuration + this->cameraMovingDelay)
	{
		cameraMovingDirection.x = 1;
		cameraMovingDirection.y = 0;

		D3DXVECTOR2 cameraPosition = D3DXVECTOR2(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
		cameraPosition += cameraMovingSpeed * dt * cameraMovingDirection;

		if (cameraPosition.x + Camera::GetInstance()->GetWidth() / 2 > cameraLimitRight)
		{
			cameraPosition.x = cameraLimitRight - Camera::GetInstance()->GetWidth() / 2;
		}

		Camera::GetInstance()->SetPosition(cameraPosition.x, cameraPosition.y);
	}


	#pragma endregion
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
	}
}
