#pragma once
#include "DynamicObject.h"

class Mine : public DynamicObject
{
private:
	bool active = false;

public:
	Mine();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();

	void OnCollisionEnter(CollisionEvent e);
};

