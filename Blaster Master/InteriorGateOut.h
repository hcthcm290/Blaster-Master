#pragma once
#include "StaticObject.h"

class InteriorGateOut : public StaticObject
{
private:
	int id_target_scene;

public:
	void Update(float dt) {}
	void Render() {}
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);

	void SetIDTargetScene(int id) { this->id_target_scene = id; }
};