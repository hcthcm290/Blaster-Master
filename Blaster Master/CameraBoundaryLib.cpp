#include "CameraBoundaryLib.h"
#include "CollisionSystem.h"

std::unordered_map<std::string, FRECT> CameraBoundaryLib::lib;

void CameraBoundaryLib::AddCameraBoundary(std::string idSection, FRECT cameraBoundary)
{
	lib[idSection] = cameraBoundary;
}

FRECT CameraBoundaryLib::getCameraBoundary(std::string id)
{
	return lib[id];
}

/// <summary>
/// Return the first CameraBoundary that target object is in
/// </summary>
/// <param name="targetObj"></param>
/// <returns></returns>
FRECT CameraBoundaryLib::GetCameraBoundary(CGameObject* targetObj)
{
	for (auto boundary : lib)
	{
		if (CollisionSystem::CheckOverlap(boundary.second, targetObj->GetCollisionBox()))
		{
			return boundary.second;
		}
	}

	return FRECT();
}

void CameraBoundaryLib::ClearLib()
{
	lib.clear();
}
