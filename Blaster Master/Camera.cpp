#include "Camera.h"

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
}

void Camera::FollowTarget()
{
	if (target == NULL) return;
	
	SetPosition(target->GetPosition().x, target->GetPosition().y);
}
