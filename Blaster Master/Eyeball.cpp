#pragma once
#include "Eyeball.h"

Eyeball::Eyeball() {
	path = new CurvedPath();
}

Eyeball::Eyeball(float x, float y, int dx, int dy) {
	this->SetPosition(x, y);

	int iDeg = rand() % 46; //0 -> 45
	int iDirection = (rand() % 2 ? 1 : -1); // -1 or 1
	int iInitAngle;
	
	if (dx != 0) { //go horizontal
		if (dx < 0) { //go left
			iInitAngle = 180;
		}
		else { //go right 
			iInitAngle = 0;
		}
	}
	else { //go vertical
		if (dy < 0) { //go up
			iInitAngle = 90;
		}
		else {	//go down
			iInitAngle = -90;
		}
	}
	path = new LinearPath((float)(iDeg * iDirection + iInitAngle));
}


void Eyeball::Update(float dt)
{
	path->Update(dt);
	vx = speed * path->GetVx();
	vy = speed * path->GetVy();
}

void Eyeball::Render()
{
	animator->Draw(20902, x, y, false);
}

#pragma region EyeballMovePath
void EyeballMovePath::Update(float dt) {
	//do nothing
}
#pragma endregion

#pragma region Linear Path
LinearPath::LinearPath(float angle) {
	vx = 1 * cos(Deg2Rad(angle));
	vy = -1 * sin(Deg2Rad(angle)); //y-axis is flipped
}

void LinearPath::Update(float angle) {

}
#pragma endregion

#pragma region Curved Path
#pragma endregion