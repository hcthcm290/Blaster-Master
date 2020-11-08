#pragma once
#include "GameObject.h"

class Camera: public CGameObject
{
private:
	Camera() {};

	static Camera* __instance;
	CGameObject* target;

	int width = 400;
	int height = 300;

	FRECT boundary = FRECT(1024, 1024, 2000, 2000);

	/// <summary>
	/// The area in camera let player move freely without change the camera position
	///				________________	
	///				|				|
	///				|	_______		|		THE BIG RECTANGLE IS THE CAMERA VIEW
	///				|	|      |	|	
	/// 			|	|______|	|		THE SMALL RECTANGLE IS THE FREE MOVING AREA
	///				|				|
	///				|_______________|
	/// </summary>
	FRECT freeMovingArea = FRECT(-50, -50, 20, 20);

private:
	void SnapToBoundary();
	void FollowTarget();

public:
	static Camera* GetInstance()
	{
		if (__instance == nullptr)
		{
			__instance = new Camera();
			__instance->SetPosition(1024, 1150);
		}
		return __instance;
	}

	void SetTarget(CGameObject* target)
	{
		__instance->target = target;
	}

	void Update(float dt)
	{
		FollowTarget();
		SnapToBoundary();
	}

	void Render() {}

	FRECT GetCollisionBox() 
	{
		FRECT colRect;
		colRect.left = x - width / 2;
		colRect.right = x + width / 2;
		colRect.top = y - height / 2;
		colRect.bottom = y + height / 2;

		return colRect;
	}

	void SetWidth(int width) { __instance->width = width; }
	void SetHeight(int height) { __instance->height = height; }

	int GetWidth() { return __instance->width; }
	int GetHeight() { return __instance->height; }

	void SetCameraBoundary(FRECT boundary)
	{
		__instance->boundary = boundary;
	}
};
