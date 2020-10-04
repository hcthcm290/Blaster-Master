#pragma once
#include <unordered_map>
#include <d3d9.h>
#include <string>

class CTextures
{
	static CTextures* __instance;

	std::unordered_map<int, LPDIRECT3DTEXTURE9> textures;

private:
	CTextures();

public:
	static CTextures* GetInstance();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(int i);
};