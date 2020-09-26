#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(std::string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath, transparentColor);
}

LPDIRECT3DTEXTURE9 CTextures::Get(std::string i)
{
	return textures[i];
}



