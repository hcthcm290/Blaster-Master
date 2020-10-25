#pragma once
#include "StaticObject.h"
#include "RigidBody.h"

class ColliableBrick: public StaticObject, public RigidBody
{
	FRECT GetCollisionBox();
};