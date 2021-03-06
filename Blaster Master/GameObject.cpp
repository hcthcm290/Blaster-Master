#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"


D3DVECTOR CGameObject::GetPosition()
{
	D3DVECTOR position;

	position.x = x;
	position.y = y;
	position.z = 0;

	return position;
}

float CGameObject::GetZIndex()
{
	return this->z_index;
}

void CGameObject::SetZIndex(float value)
{
	this->z_index = value;
}

CGameObject::CGameObject()
{
	x = y = 0;
	currentState = 0;
}

void CGameObject::Update(float dt)
{
}

void CGameObject::Render()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::OnOverlap(CGameObject* obj)
{
}
