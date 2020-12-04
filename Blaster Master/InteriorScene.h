#pragma once
#include "PlayScene.h"

class InteriorScene: public CPlayScene
{

public:
	InteriorScene(int id, LPCWSTR filePath);
	void Unload();
protected:
	virtual void _ParseSection_OBJECTS(string line);
	virtual void UpdateSwitchSection(float dt);
};
