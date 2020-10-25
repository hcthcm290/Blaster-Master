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

#define MAX_FRAME_RATE 1000
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


void Update(DWORD dt)
{
    game->GetCurrentScene()->Update(dt);
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
    DWORD tickPerFrame = 0.001;

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