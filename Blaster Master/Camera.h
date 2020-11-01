#pragma once
#include "GameObject.h"

class Camera: public CGameObject
{
private:
	Camera() {};

	static Camera* __instance;
	CGameObject* target;

	int width = 800;
	int height = 600;

	FRECT boundary;

public:
	static Camera* GetInstance()
	{
		if (__instance == nullptr)
		{
			__instance = new Camera();
		}
		return __instance;
	}

	void SetTarget(CGameObject* target)
	{
		__instance->target = target;
	}

	void Update(float dt)
	{
		SetPosition(target->GetPosition().x, target->GetPosition().y);
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

Camera* Camera::__instance = NULL;