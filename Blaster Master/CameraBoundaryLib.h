#pragma once
#include <unordered_map>
#include <string>
#include "RECT.h"

class CameraBoundaryLib
{
private:
	static std::unordered_map<std::string, FRECT> lib;

public:
	static void AddCameraBoundary(std::string idSection, FRECT cameraBoundary);
	static FRECT getCameraBoundary(std::string id);
};