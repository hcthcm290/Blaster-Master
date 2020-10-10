#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"


CGameObject::CGameObject()
{
	x = y = 0;
	currentState = 0;
}

void CGameObject::Update(DWORD dt)
{
}

void CGameObject::Render()
{
}

CGameObject::~CGameObject()
{
}