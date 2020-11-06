#pragma once
#include "DynamicObject.h"

class Mine : public DynamicObject
{
private:
	bool active = true;
	int count;

public:
	Mine();
	void Update(float dt);
	void Render();

	FRECT GetCollisionBox();

	void OnCollisionEnter(CollisionEvent e);
};

