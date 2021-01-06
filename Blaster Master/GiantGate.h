#pragma once
#include "StaticObject.h"

class GiantGate : public StaticObject
{
protected:
	int idtargetScene;

public: 
	int GetIDTargetScene() { return this->idtargetScene; }
	void SetIDTargetScene(int value) { this->idtargetScene = value; }

public:
	FRECT GetCollisionBox();
	void Update(float dt);
	void Render();
};