#pragma once
#include "StaticObject.h"
#include "Animator.h"

class OutroController: public StaticObject
{
private:
	#pragma region RedScene
	
	int RedSceneAnimationID = 211600;
	float RedSceneLifetime = 14;
	D3DXVECTOR2 RedAnimationPosition = D3DXVECTOR2(0, 0);
	Animator RedSceneAnimator;

	#pragma endregion

	#pragma region Mountain
	
	int mountainSpriteID = 111700;
	int mountainFallingSpeed = 7;
	int mountainLifetime = 4;
	int mountainDelayFalling = 1;
	D3DXVECTOR2 mountainPosition = D3DXVECTOR2(2, 12);

	#pragma endregion

	#pragma region CameraController
	
	float cameraMovingSpeed = 50;
	int cameraLimitRight = 1533; // camera's CollisionBox.right cannot get over this value;
	float cameraShakingDuration = 3.8;
	float cameraShakingInterval = 0.02;
	float cameraDelayShaking = 1.2;
	float cameraShakingCountTime = 0;
	D3DXVECTOR2 cameraMovingDirection = D3DXVECTOR2(0, 0);
	D3DXVECTOR2 cameraPosition = D3DXVECTOR2(0, 0);

	#pragma endregion

	float CountTime = 0.0f;
public:
	OutroController();
	void Update(float dt);
	void Render();
};