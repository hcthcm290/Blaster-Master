#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "RECT.h"
#include "CollisionEvent.h"

using namespace std;

typedef CGameObject* LPGAMEOBJECT;
class CGameObject
{
protected:
	float x;
	float y;

protected:
	int currentState;

public:

	/// <summary>
	///		<para>
	///			Set position for game object.
	///		</para>
	///		<para>
	///			Becarefull when using this function, the collision system may process wrong if
	///			position of the game object is set directly, we recommend you update the velocity (if the object is the DynamicObject), 
	///			the system will automatically update the position every frame based on the velocity
	///		</para>
	/// </summary>
	/// <param name="x">position x</param>
	/// <param name="y">position y</param>
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void SetState(int state) { this->currentState = state; }
	virtual int GetState() { return this->currentState; }

	D3DVECTOR GetPosition();

	CGameObject();

	virtual FRECT GetCollisionBox() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	~CGameObject();

public:
	virtual void OnCollisionEnter(CollisionEvent e) {}
	virtual void OnCollisionExit(CollisionEvent e) {}
	
};

