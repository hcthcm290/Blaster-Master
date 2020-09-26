#pragma once
#include <unordered_map>
#include <d3d9.h>
#include <string>

class CTextures
{
	static CTextures* __instance;

	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures;

private:
	CTextures();

public:
	static CTextures* GetInstance();
	void Add(std::string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(std::string i);
};