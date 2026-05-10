#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    bool Init(HINSTANCE hInstance, HWND hWnd);
    void Update();
    void Cleanup();

    bool IsKeyDown(int key);
    int  GetMouseDeltaX();
    int  GetMouseDeltaY();

private:
    LPDIRECTINPUT8       m_pDI;
    LPDIRECTINPUTDEVICE8 m_pKeyboard;
    LPDIRECTINPUTDEVICE8 m_pMouse;

    char         m_KeyState[256];
    DIMOUSESTATE m_MouseState;
};