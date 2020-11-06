#pragma once 
#include "Behavior.h"
#include "Jason.h"

class Behavior_Jason : public Behavior {
private:
	int speed = 20;
	//Jason* object;

	D3DVECTOR v;
public:
	Behavior_Jason(Jason* obj);
	void Start();
	void Update(DWORD dt);
};