#pragma once
#include "Sophia.h"
#include "Jason.h"
#include "BigJason.h"

// The eye is a singleton that know about all the playable object
// Any playable object when create MUST notify the eye
// Any playable object want to know each other can ask the eye
class TheEye
{
private:
	TheEye() {}

private:
	static TheEye* __instance;

private:
	Sophia* sophia = NULL;
	Jason* jason = NULL;
	BigJason* bigJason = NULL;

public:
	static TheEye* GetInstance();

	void SetSophia(Sophia* sophia) { this->sophia = sophia; }
	Sophia* GetSophia() { return this->sophia; }

	void SetJason(Jason* jason) { this->jason = jason; }
	Jason* GetJason() { return this->jason; }

	void SetBigJason(BigJason* bigJason) { this->bigJason = bigJason; }
	BigJason* GetBigJason() { return this->bigJason; }
};