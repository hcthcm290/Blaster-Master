#pragma once
#include "StaticObject.h"
#include "Animator.h"
#include "Sound.h"

// Flashing area in Interior
// When player hit the BossArea, it start flashing and move player to Boss scene
class BossArea: public StaticObject
{
protected:
	int backgroundAnimationID; // the flashing that appear as the background to the player
	D3DXVECTOR2 backgroundPosition;

	int foregroundAnimationID; // the same as above but as foreground
	D3DXVECTOR2 foregroundPosition;

	int blackscreenSpriteID; // the black screen that appear between scene background and the player

	int width;
	int height;

	float Countime; // count total time have passed since player enter the boss area, reset when player exit the area
	float MaxCountime = 8; // when Countime reach MaxCountime, boss appear

	bool bossSpawned = false;

	D3DXVECTOR2 BossInitPosition;

	Animator backgroundAnimator;
	Animator foregroundAnimator;

private:
	int blackscreenAlphaValue;

protected:
	const float delayFlashing = 0.4;
	const float delayFadding = 5.8;
	const float faddingTime = 0.4;

public:
	void SetWidth(int value) { this->width = value; }
	void SetHeight(int value) { this->height = value; }
	void SetBossInitPosition(D3DXVECTOR2 value) { this->BossInitPosition = value; }
	void SetBackgroundAnimationID(int value) { this->backgroundAnimationID = value; this->backgroundAnimator.AddAnimation(value); }
	void SetBackgroundPosition(D3DXVECTOR2 value) { this->backgroundPosition = value; }
	void SetForegroundPosition(D3DXVECTOR2 value) { this->foregroundPosition = value; }
	void SetForegroundAnimationID(int value) { this->foregroundAnimationID = value; this->foregroundAnimator.AddAnimation(value); }
	void SetBlackscreenSpriteID(int value) { this->blackscreenSpriteID = value; }

	virtual FRECT GetCollisionBox();
	virtual void Update(float dt);
	virtual void Render();
};