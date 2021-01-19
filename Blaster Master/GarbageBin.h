#pragma once
#include "GameObject.h"
#include <vector>

class GarbageBin
{
private:
	static GarbageBin* __instance;
	GarbageBin() {};

public:
	static GarbageBin* GetInstance();

private:
	std::vector<CGameObject*> storage;

public:
	void AddToGarbageBin(CGameObject* object);
	void RemoveFromGarbageBin(CGameObject* object);
	void RestoreFromGarbageBin(CGameObject* object);
	void CleanGarbageBin();

	bool IsExistInGarbageBin(CGameObject* object);
	int NumberOfTrash();
};
