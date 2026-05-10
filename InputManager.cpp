#define DIRECTINPUT_VERSION 0x0800
#include "InputManager.h"

InputManager::InputManager()
{
    m_pDI = NULL;
    m_pKeyboard = NULL;
    m_pMouse = NULL;
    ZeroMemory(m_KeyState, sizeof(m_KeyState));
    ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

InputManager::~InputManager()
{
    Cleanup();
}

bool InputManager::Init(HINSTANCE hInstance, HWND hWnd)
{
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
        IID_IDirectInput8, (void**)&m_pDI, NULL)))
        return false;

    if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
        return false;

    m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
    m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    m_pKeyboard->Acquire();

    if (FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
        return false;

    m_pMouse->SetDataFormat(&c_dfDIMouse);
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    m_pMouse->Acquire();

    return true;
}

void InputManager::Update()
{
    if (FAILED(m_pKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState)))
    {
        m_pKeyboard->Acquire();
        ZeroMemory(m_KeyState, sizeof(m_KeyState));
    }

    if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
    {
        m_pMouse->Acquire();
        ZeroMemory(&m_MouseState, sizeof(m_MouseState));
    }
}

void InputManager::Cleanup()
{
    if (m_pKeyboard)
    {
        m_pKeyboard->Unacquire();
        m_pKeyboard->Release();
        m_pKeyboard = NULL;
    }

    if (m_pMouse)
    {
        m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = NULL;
    }

    if (m_pDI)
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
}

bool InputManager::IsKeyDown(int key)
{
    return (m_KeyState[key] & 0x80) != 0;
}

int InputManager::GetMouseDeltaX()
{
    return m_MouseState.lX;
}

int InputManager::GetMouseDeltaY()
{
    return m_MouseState.lY;
}