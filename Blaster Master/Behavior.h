#pragma once 
#include "DynamicObject.h"

class Behavior {
private:
	DynamicObject* object;
public:
	Behavior();
	//Behavior(DynamicObject* obj);
	virtual void Start();
	virtual void Update(DWORD dt);
};