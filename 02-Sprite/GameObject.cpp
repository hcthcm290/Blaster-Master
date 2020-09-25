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
	vx = 0.07f;

	for (std::pair<std::string, CAnimationInfo*> element : animationInfos)
	{
		CAnimation* anim = new CAnimation(element.second);
		animations[element.first] = anim;
	}
}

void CGameObject::Update(DWORD dt)
{
	x += vx*dt;
	if ((vx>0 && x > 290) || (x < 0 && vx<0)) vx=-vx;

}

void CGameObject::Render()
{
	LPANIMATION ani;
	//if (vx>0) ani = animations[0]; else ani = animations[1];

	/*if (vx>0) ani = CAnimations::GetInstance()->Get(500); 
	else ani = CAnimations::GetInstance()->Get(501);*/

	ani = animations["Running"];
	//ani = animations[0];
	if (vx > 0) ani->Render(x, y, true);
	else ani->Render(x, y, false);
}

CGameObject::~CGameObject()
{
	for (std::pair<std::string, CAnimation*> element : animations)
	{
		delete element.second;
	}
}