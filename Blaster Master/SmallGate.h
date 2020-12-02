#pragma once
#include "GameObject.h"

class SmallGate : public CGameObject
{
private:
	int id_target_scene;
public:
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render() {}
	void SetIDTargetScene(int id) { this->id_target_scene = id; }
};