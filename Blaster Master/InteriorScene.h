#pragma once
#include "PlayScene.h"

class InteriorScene: public CPlayScene
{
private:
	bool ResetHealth = false;
	bool AdaptJasonHealth = true;

public:
	InteriorScene(int id, LPCWSTR filePath);
	void Unload();
protected:
	virtual void _ParseSection_OBJECTS(string line);
	virtual void UpdateSwitchSection(float dt);
	virtual void UpdateFaddingOut(float dt);
	virtual void InitGUI();
};
