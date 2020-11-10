#pragma once
#include "StaticObject.h"

class BigGate : public StaticObject
{
public:
	D3DXVECTOR2 shift_direction;
	float shift_speed;
	FRECT new_boundary_camera;
	float shift_time1;
	float shift_time2;

	D3DXVECTOR2 teleport_delta;

public:
	FRECT GetCollisionBox();
	void OnCollisionEnter(CollisionEvent e);
	void Render() {};
};