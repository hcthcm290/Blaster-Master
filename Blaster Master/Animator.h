#pragma once
#include "Sprites.h"

class Animator
{
protected:
	int curFrame = 0;
	int curAnimationID = -1;

protected:
	unordered_map<int, LPANIMATION> animationSet;

public:
	virtual void Draw(int animationID, float x, float y, bool flipX);
	virtual void Draw(int animationID, float x, float y, bool flipX, float rotation);
	virtual void Draw(int animationID, float x, float y, bool flipX, float rotation, D3DCOLOR Color);
	virtual void Draw(int animationID, float x, float y, bool flipX, float rotation, D3DCOLOR Color, float z_index);
	virtual void AddAnimation(int id);
};