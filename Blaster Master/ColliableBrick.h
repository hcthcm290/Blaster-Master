#pragma once
#include "StaticObject.h"
#include "RigidBody.h"

class ColliableBrick: public StaticObject, public RigidBody
{
private:
	int width = 16;
	int height = 16;

public:
	FRECT GetCollisionBox();
	void SetWidth(int width) { this->width = width; }
	void SetHeight(int height) { this->height = height; }

	int GetWidth() { return width; }
	int GetHeight() { return height; }
};