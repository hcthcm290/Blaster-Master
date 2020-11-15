#include "CameraBoundaryLib.h"

std::unordered_map<std::string, FRECT> CameraBoundaryLib::lib;

void CameraBoundaryLib::AddCameraBoundary(std::string idSection, FRECT cameraBoundary)
{
	lib[idSection] = cameraBoundary;
}

FRECT CameraBoundaryLib::getCameraBoundary(std::string id)
{
	return lib[id];
}
