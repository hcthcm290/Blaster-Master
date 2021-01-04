#pragma once
#include "PlayScene.h"

class InteriorScene: public CPlayScene
{
private:
	bool ResetHealth = false;
	bool AdaptJasonHealth = true;
	
protected:
	bool isFightingBoss = false;

public:
	InteriorScene(int id, LPCWSTR filePath);
	void Unload();
	~InteriorScene();
	void SwitchSection(BigGate* gate);
	void SetFightingBoss(bool value) { this->isFightingBoss = value; }
	virtual void Render();

protected:
	virtual void _ParseSection_OBJECTS(string line);
	virtual void UpdateSwitchSection(float dt);
	virtual void UpdateFaddingOut(float dt);
	virtual void InitGUI();


};
