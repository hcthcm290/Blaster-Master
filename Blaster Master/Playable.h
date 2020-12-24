#pragma once

class Playable
{
public:
	virtual bool IsInvulnerable() = 0;
	virtual float GetEnterGateSpeed() = 0;
	virtual bool IsDead() = 0;
};
