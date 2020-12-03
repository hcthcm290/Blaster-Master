#pragma once
#include "StaticObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"



class Spike : public StaticObject
{
private:
	int len;
public:
	
	Spike(int);
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};