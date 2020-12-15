#pragma once

#include "Debug.h"

class CheatTool {
private:
	static CheatTool* __instance;
public:
	static CheatTool* GetInstance();
};