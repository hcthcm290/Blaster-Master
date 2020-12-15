#pragma once

#include "CheatTool.h"
#include "DInput.h"

CheatTool* CheatTool::GetInstance() {
	if (__instance == NULL) __instance = new CheatTool();
	return __instance;
}