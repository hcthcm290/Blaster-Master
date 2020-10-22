#pragma once
#include "DynamicObject.h"

#define ORB_COLLISION_BOX_WIDTH 18
#define ORB_COLLISION_BOX_HEIGHT 18

class Orb : public DynamicObject
{
public:
	D3DRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};