#pragma once
#include "DynamicObject.h"
#include "Bullet.h"
class VisionBox :
    public DynamicObject, public Bullet
{
protected:
	int left, right, top, bottom;
public:
	static int count;
	VisionBox(int left, int right, int top, int bottom);
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Update(float dt);
	void Render();
};

