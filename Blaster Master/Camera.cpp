#include "Camera.h"
#include "PlayScene.h"
#include "InteriorScene.h"

Camera* Camera::__instance = NULL;

void Camera::SnapToBoundary()
{
	FRECT cameraRECT = GetCollisionBox();

	if (cameraRECT.left < boundary.left)
	{
		x += boundary.left - cameraRECT.left;
	}
	
	if (cameraRECT.right > boundary.right)
	{
		x -= cameraRECT.right - boundary.right;
	}

	if (cameraRECT.top < boundary.top)
	{
		y += boundary.top - cameraRECT.top;
	}
	
	if (cameraRECT.bottom > boundary.bottom)
	{
		y -= cameraRECT.bottom - boundary.bottom;
	}

	x = int(x);
	y = int(y);
}

void Camera::FollowTarget()
{
	target = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (target == NULL) return;

	int deltaX = target->GetPosition().x - x;
	int deltaY = target->GetPosition().y - y;

	if (dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene()) != NULL)
	{
		freeMovingArea = freeMovingInterior;
	}
	else
	{
		freeMovingArea = freeMovingSideview;
	}

	if (deltaX > freeMovingArea.right)
	{
		x += deltaX - freeMovingArea.right;
	}

	if (deltaX < freeMovingArea.left)
	{
		x += deltaX - freeMovingArea.left;
	}

	if (deltaY > freeMovingArea.bottom)
	{
		y += deltaY - freeMovingArea.bottom; 
	}

	if (deltaY < freeMovingArea.top)
	{
		y += deltaY - freeMovingArea.top;
	}
	
	//SetPosition(target->GetPosition().x, target->GetPosition().y);
}

