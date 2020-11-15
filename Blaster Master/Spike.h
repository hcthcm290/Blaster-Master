#pragma once
#include "StaticObject.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "GameObject.h"

#define spikeSprite State::_SPIKE_


class Spike : public StaticObject
{
private:
	int state;
	int flipRotation=0;
public:
	Spike() {};
	Spike(int);
	void Update(float dt);
	void Render();
	void OnCollisionEnter(CollisionEvent e);
	FRECT GetCollisionBox();
};