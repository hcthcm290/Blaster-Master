#include <Windows.h>
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "Orb.h"
#include "Jumper.h"
#include "Dome.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <vector>

#define MAX_FRAME_RATE 60
#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 255)

#define ORB_WIDTH 18
#define ORB_HEIGHT 18

#define JUMPER_HEIGHT 26
#define JUMPER_WIDTH 17

#define DOME_HEIGHT 17
#define DOME_WIDTH 18

Orb* orb;
Jumper* jumper;
Dome* dome;

CGame* game;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HWND CreateGameWindow(HINSTANCE hInstance, INT Width, INT Height)
{
    const wchar_t CLASS_NAME[] = L"WINDOW CLASS";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Blaster Master",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    ShowWindow(hwnd, SW_SHOW);

    return hwnd;
}

void LoadOrb()
{
    CTextures* textures = CTextures::GetInstance();

    textures->Add(1, L"Textures\\FlyingEye.png", D3DCOLOR_RGBA(255, 0, 255, 255));

    CSprites* sprites = CSprites::GetInstance();

    LPDIRECT3DTEXTURE9 orbTexture = textures->Get(1);

    sprites->Add(1, 18 * 0, 0, 18 * 1, 18, orbTexture);
    sprites->Add(2, 18 * 1, 0, 18 * 2, 18, orbTexture);
    sprites->Add(3, 18 * 2, 0, 18 * 3, 18, orbTexture);
    sprites->Add(4, 18 * 3, 0, 18 * 4, 18, orbTexture);
    sprites->Add(5, 18 * 4, 0, 18 * 5, 18, orbTexture);

    CAnimation* OrbAnimSpinning = new CAnimation(100);
    OrbAnimSpinning->Add(1);
    OrbAnimSpinning->Add(2);
    OrbAnimSpinning->Add(3);
    OrbAnimSpinning->Add(4);
    OrbAnimSpinning->Add(5);

    CAnimation* OrbAnimIdle = new CAnimation(100);
    OrbAnimSpinning->Add(1);

    CAnimations::GetInstance()->Add(1000, OrbAnimIdle);
    CAnimations::GetInstance()->Add(1001, OrbAnimSpinning);
}

void LoadJumper()
{
    CTextures* textures = CTextures::GetInstance();

    textures->Add(2, L"Textures\\Jumper.png", D3DCOLOR_RGBA(255, 0, 255, 255));

    CSprites* sprites = CSprites::GetInstance();

    LPDIRECT3DTEXTURE9 orbTexture = textures->Get(2);

    sprites->Add(11, JUMPER_WIDTH * 0, 0, JUMPER_WIDTH * 1, JUMPER_HEIGHT, orbTexture);
    sprites->Add(12, JUMPER_WIDTH * 1, 0, JUMPER_WIDTH * 2, JUMPER_HEIGHT, orbTexture);
    sprites->Add(13, JUMPER_WIDTH * 2, 0, JUMPER_WIDTH * 3, JUMPER_HEIGHT, orbTexture);

    CAnimation* moving = new CAnimation(100);
    moving->Add(11);
    moving->Add(12);
    moving->Add(13);

    CAnimation* idle = new CAnimation(100);
    idle->Add(11);

    CAnimations::GetInstance()->Add(1002, moving);
    CAnimations::GetInstance()->Add(2000, idle);
}

void LoadDome()
{
    CTextures* textures = CTextures::GetInstance();

    textures->Add(3, L"Textures\\Dome.png", D3DCOLOR_RGBA(255, 0, 255, 255));

    CSprites* sprites = CSprites::GetInstance();

    LPDIRECT3DTEXTURE9 domeTexture = textures->Get(3);

    sprites->Add(21, DOME_WIDTH * 0, 0, DOME_WIDTH * 1, DOME_HEIGHT, domeTexture);
    sprites->Add(22, DOME_WIDTH * 1, 0, DOME_WIDTH * 2, DOME_HEIGHT, domeTexture);

    CAnimation* moving = new CAnimation(100);
    moving->Add(21);
    moving->Add(22);

    CAnimation* idle = new CAnimation(100);
    idle->Add(21);

    CAnimations::GetInstance()->Add(1003, moving);
    CAnimations::GetInstance()->Add(3000, idle);
}

void LoadResource()
{
    LoadOrb();
    LoadJumper();
    LoadDome();
}

void CreateGameObject()
{
    orb = new Orb();
    orb->SetPosition(100, 100);
    jumper = new Jumper();
    jumper->SetPosition(200, 100);
    dome = new Dome();
    dome->SetPosition(300, 100);
}

void Update(DWORD dt)
{
    game->GetCurrentScene()->Update(dt);
    orb->Update(dt);
    dome->Update(dt);
    jumper->Update(dt);
}

void Render()
{
    CGame* game = CGame::GetInstance();

    LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
    LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
    LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

    if (d3ddv->BeginScene())
    {
        // Clear back buffer with a color
        d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

        spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

        //orb->Render();
        //jumper->Render();
        //dome->Render();

        game->GetCurrentScene()->Render();

        spriteHandler->End();
        d3ddv->EndScene();
    }

    // Display back buffer content to the screen
    d3ddv->Present(NULL, NULL, NULL, NULL);
}

int Run()
{
    MSG msg;
    int done = 0;
    DWORD frameStart = GetTickCount();
    DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) done = 1;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DWORD now = GetTickCount();

        // dt: the time between (beginning of last frame) and now
        // this frame: the frame we are about to render
        DWORD dt = now - frameStart;

        CGame::GetInstance()->ProcessKeyboard();

        if (dt >= tickPerFrame)
        {
            frameStart = now;
            Update(dt);
            Render();
        }
        else
            Sleep(tickPerFrame - dt);
    }

    return 1;
}



INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    HWND hwnd = CreateGameWindow(hInstance, 800, 600);

    game = CGame::GetInstance();

    game->Init(hwnd);
    game->Load(L"game-info.txt");

    Run();

    return 0;
}