#include <Windows.h>
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "Orb.h"
#include "Jumper.h"

#define MAX_FRAME_RATE 60
#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 255)

#define ORB_WIDTH 18
#define ORB_HEIGHT 18

#define JUMPER_HEIGHT 26
#define JUMPER_WIDTH 17

Orb* orb;
Jumper* jumper;

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

    textures->Add("Flying eye", L"Textures\\FlyingEye.png", D3DCOLOR_RGBA(255, 0, 255, 255));

    CSprites* sprites = CSprites::GetInstance();

    LPDIRECT3DTEXTURE9 orbTexture = textures->Get("Flying eye");

    sprites->Add("Orb_01", 18 * 0, 0, 18 * 1, 18, orbTexture);
    sprites->Add("Orb_02", 18 * 1, 0, 18 * 2, 18, orbTexture);
    sprites->Add("Orb_03", 18 * 2, 0, 18 * 3, 18, orbTexture);
    sprites->Add("Orb_04", 18 * 3, 0, 18 * 4, 18, orbTexture);
    sprites->Add("Orb_05", 18 * 4, 0, 18 * 5, 18, orbTexture);

    CAnimationInfo* OrbAnimSpinning = new CAnimationInfo(200);
    OrbAnimSpinning->Add("Orb_01");
    OrbAnimSpinning->Add("Orb_02");
    OrbAnimSpinning->Add("Orb_03");
    OrbAnimSpinning->Add("Orb_04");
    OrbAnimSpinning->Add("Orb_05");

    CAnimationInfo* OrbAnimIdle = new CAnimationInfo(200);
    OrbAnimSpinning->Add("Orb_01");

    Orb::AddAnimation("idle", OrbAnimSpinning);
    Orb::AddAnimation("Spinning", OrbAnimSpinning);
}

void LoadJumper()
{
    CTextures* textures = CTextures::GetInstance();

    textures->Add("Jumper", L"Textures\\Jumper.png", D3DCOLOR_RGBA(255, 0, 255, 255));

    CSprites* sprites = CSprites::GetInstance();

    LPDIRECT3DTEXTURE9 orbTexture = textures->Get("Jumper");

    sprites->Add("JUMPER_01", JUMPER_WIDTH * 0, 0, JUMPER_WIDTH * 1, JUMPER_HEIGHT, orbTexture);
    sprites->Add("JUMPER_02", JUMPER_WIDTH * 1, 0, JUMPER_WIDTH * 2, JUMPER_HEIGHT, orbTexture);
    sprites->Add("JUMPER_03", JUMPER_WIDTH * 2, 0, JUMPER_WIDTH * 3, JUMPER_HEIGHT, orbTexture);

    CAnimationInfo* moving = new CAnimationInfo(200);
    moving->Add("JUMPER_01");
    moving->Add("JUMPER_02");
    moving->Add("JUMPER_03");

    CAnimationInfo* idle = new CAnimationInfo(200);
    idle->Add("JUMPER_01");

    Jumper::AddAnimation("idle", idle);
    Jumper::AddAnimation("moving", moving);
}

void LoadResource()
{
    LoadOrb();
    LoadJumper();
}

void CreateGameObject()
{
    orb = new Orb();
    orb->SetPosition(100, 100);
    jumper = new Jumper();
    jumper->SetPosition(200, 100);
}

void Update(DWORD dt)
{

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

        orb->Render();
        jumper->Render();

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

    CGame::GetInstance()->Init(hwnd);

    LoadResource();

    CreateGameObject();

    Run();

    return 0;
}