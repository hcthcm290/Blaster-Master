#pragma once
#include "StaticObject.h"
#include "CollisionSystem.h"
#include "GameObject.h"



class Spike_Interior : public StaticObject
{
private:
	int width,height;
public:

	Spike_Interior(int,int);
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};