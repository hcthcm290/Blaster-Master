#pragma once
#include "PlayScene.h"

class InteriorScene: public CPlayScene
{

public:
	InteriorScene(int id, LPCWSTR filePath);
	~InteriorScene();
protected:
	virtual void _ParseSection_OBJECTS(string line);
};
