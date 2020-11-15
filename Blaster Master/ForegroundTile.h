#pragma once
#include "StaticObject.h"

class ForegroundTile : public StaticObject
{
public:
	FRECT GetCollisionBox();
};