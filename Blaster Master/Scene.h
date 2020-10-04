#pragma once
#include <d3dx9.h>

class CScene
{
protected:
	int id;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CScene* LPSCENE;