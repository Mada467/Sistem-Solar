#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include "Camera.h"
#include "Skybox.h"
#include "Planet.h"
#include "InputManager.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

LPDIRECT3D9       g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

Camera       g_Camera;
Skybox       g_Skybox;
Planet       g_Soare;
Planet       g_Pamant;
Planet       g_Luna;
InputManager g_Input;

DWORD g_dwLastTime = 0;

HRESULT InitD3D(HWND hWnd)
{
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferWidth = WINDOW_WIDTH;
    d3dpp.BackBufferHeight = WINDOW_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
            return E_FAIL;
    }

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(30, 30, 30));

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(light));
    light.Type = D3DLIGHT_POINT;
    light.Diffuse = { 1.0f, 0.95f, 0.8f, 1.0f };
    light.Specular = { 1.0f, 1.0f,  1.0f, 1.0f };
    light.Ambient = { 0.1f, 0.1f,  0.1f, 1.0f };
    light.Position = { 0.0f, 0.0f,  0.0f };
    light.Range = 1000.0f;
    light.Attenuation0 = 1.0f;
    light.Attenuation1 = 0.0f;
    light.Attenuation2 = 0.0f;
    g_pd3dDevice->SetLight(0, &light);
    g_pd3dDevice->LightEnable(0, TRUE);

    return S_OK;
}

void InitObjects(HWND hWnd, HINSTANCE hInst)
{
    g_Input.Init(hInst, hWnd);

    g_Camera.SetPosition(0.0f, 10.0f, -80.0f);
    g_Camera.SetTarget(0.0f, 0.0f, 0.0f);

    g_Skybox.Load(g_pd3dDevice,
        "textures/space.jpg", "textures/space.jpg",
        "textures/space.jpg", "textures/space.jpg",
        "textures/space.jpg", "textures/space.jpg");

    g_Soare.Load(g_pd3dDevice, "soare.x", "textures/soare.jpg");
    g_Soare.SetPosition(0.0f, 0.0f, 0.0f);
    g_Soare.SetScale(5.0f);
    g_Soare.SetRotationSpeed(0.2f);
    g_Soare.SetOrbitRadius(0.0f);

    g_Pamant.Load(g_pd3dDevice, "pamant.x", "textures/pamant.jpg");
    g_Pamant.SetScale(3.0f);
    g_Pamant.SetRotationSpeed(0.5f);
    g_Pamant.SetOrbitSpeed(0.3f);
    g_Pamant.SetOrbitRadius(20.0f);
    g_Pamant.SetOrbitCenter(0.0f, 0.0f, 0.0f);

    g_Luna.Load(g_pd3dDevice, "luna.x", "textures/luna.jpg");
    g_Luna.SetScale(0.6f);
    g_Luna.SetRotationSpeed(0.3f);
    g_Luna.SetOrbitSpeed(1.0f);
    g_Luna.SetOrbitRadius(6.0f);

    g_dwLastTime = timeGetTime();

    PlaySound("sounds\\space.wav.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void HandleInput(float deltaTime)
{
    g_Input.Update();

    // Camera - miscare
    if (g_Input.IsKeyDown(DIK_W)) g_Camera.MoveForward(10.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_S)) g_Camera.MoveForward(-10.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_A)) g_Camera.MoveRight(-10.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_D)) g_Camera.MoveRight(10.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_Q)) g_Camera.MoveUp(10.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_E)) g_Camera.MoveUp(-10.0f * deltaTime);

    // Camera - rotire cu mouse
    float mouseX = g_Input.GetMouseDeltaX() * 0.005f;
    float mouseY = g_Input.GetMouseDeltaY() * 0.005f;
    if (mouseX != 0) g_Camera.RotateYaw(mouseX);
    if (mouseY != 0) g_Camera.RotatePitch(mouseY);

    // Mesh (Pamant) - translatie fata-spate, stanga-dreapta, sus-jos
    if (g_Input.IsKeyDown(DIK_UP))       g_Pamant.Translate(0.0f, 0.0f, 5.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_DOWN))     g_Pamant.Translate(0.0f, 0.0f, -5.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_LEFT))     g_Pamant.Translate(-5.0f * deltaTime, 0.0f, 0.0f);
    if (g_Input.IsKeyDown(DIK_RIGHT))    g_Pamant.Translate(5.0f * deltaTime, 0.0f, 0.0f);
    if (g_Input.IsKeyDown(DIK_PRIOR))    g_Pamant.Translate(0.0f, 5.0f * deltaTime, 0.0f);
    if (g_Input.IsKeyDown(DIK_NEXT))     g_Pamant.Translate(0.0f, -5.0f * deltaTime, 0.0f);

    // Mesh (Pamant) - rotire fata de propriile axe
    if (g_Input.IsKeyDown(DIK_NUMPAD4))  g_Pamant.AddRotationY(-2.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_NUMPAD6))  g_Pamant.AddRotationY(2.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_NUMPAD8))  g_Pamant.AddRotationX(-2.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_NUMPAD2))  g_Pamant.AddRotationX(2.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_NUMPAD7))  g_Pamant.AddRotationZ(-2.0f * deltaTime);
    if (g_Input.IsKeyDown(DIK_NUMPAD9))  g_Pamant.AddRotationZ(2.0f * deltaTime);
}

void Update()
{
    DWORD dwCurrentTime = timeGetTime();
    float deltaTime = (dwCurrentTime - g_dwLastTime) / 1000.0f;
    g_dwLastTime = dwCurrentTime;

    HandleInput(deltaTime);

    g_Soare.Update(deltaTime);
    g_Pamant.Update(deltaTime);

    D3DXVECTOR3 pamantPos = g_Pamant.GetPosition();
    g_Luna.SetOrbitCenter(pamantPos.x, pamantPos.y, pamantPos.z);
    g_Luna.Update(deltaTime);

    g_Camera.Update(g_pd3dDevice);
}

void Render()
{
    if (NULL == g_pd3dDevice) return;

    g_pd3dDevice->Clear(0, NULL,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        D3DXVECTOR3 camPos = g_Camera.GetPosition();
        g_Skybox.Render(g_pd3dDevice, camPos);

        g_Soare.Render(g_pd3dDevice);
        g_Pamant.Render(g_pd3dDevice);
        g_Luna.Render(g_pd3dDevice);

        g_pd3dDevice->EndScene();
    }

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Cleanup()
{
    g_Input.Cleanup();
    g_Skybox.Cleanup();
    g_Soare.Cleanup();
    g_Pamant.Cleanup();
    g_Luna.Cleanup();

    if (g_pd3dDevice) g_pd3dDevice->Release();
    if (g_pD3D)       g_pD3D->Release();
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            Cleanup();
            PostQuitMessage(0);
        }
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "Sistem Solar", NULL };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow("Sistem Solar", "Sistem Solar - DirectX 9",
        WS_OVERLAPPEDWINDOW, 100, 100,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    if (SUCCEEDED(InitD3D(hWnd)))
    {
        InitObjects(hWnd, hInst);

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        MSG mssg;
        PeekMessage(&mssg, NULL, 0, 0, PM_NOREMOVE);

        while (mssg.message != WM_QUIT)
        {
            if (PeekMessage(&mssg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&mssg);
                DispatchMessage(&mssg);
            }
            else
            {
                Update();
                Render();
            }
        }
    }

    UnregisterClass("Sistem Solar", wc.hInstance);
    return 0;
}