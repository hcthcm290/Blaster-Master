#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

std::unordered_map<string, CAnimationInfo*> CGameObject::animationInfos;

void CGameObject::AddAnimation(string id, CAnimationInfo* animInfo)
{
	animationInfos[id] = animInfo;
}

CGameObject::CGameObject()
{
	x = y = 0;

	for (std::pair<std::string, CAnimationInfo*> element : animationInfos)
	{
		CAnimation* anim = new CAnimation(element.second);
		animations[element.first] = anim;
	}
}

void CGameObject::Update(DWORD dt)
{
}

void CGameObject::Render()
{
}

CGameObject::~CGameObject()
{
	for (std::pair<std::string, CAnimation*> element : animations)
	{
		delete element.second;
	}
}