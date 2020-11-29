#pragma once
#include "DynamicObject.h"
#include "Playable.h"

class BigJason : public DynamicObject, public Playable
{
public:
	BigJason();
	void Update(float dt);
	void Render();
	virtual FRECT GetCollisionBox();
	bool IsInvulnerable();
	float GetEnterGateSpeed();
};
