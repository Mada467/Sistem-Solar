#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
public:
    Camera();

    void SetPosition(float x, float y, float z);
    void SetTarget(float x, float y, float z);
    void MoveForward(float delta);
    void MoveRight(float delta);
    void MoveUp(float delta);
    void RotateYaw(float angle);
    void RotatePitch(float angle);
    void Update(LPDIRECT3DDEVICE9 pDevice);
    D3DXVECTOR3 GetPosition();

private:
    D3DXVECTOR3 m_Position;
    D3DXVECTOR3 m_Target;
    D3DXVECTOR3 m_Up;
    float       m_fYaw;
    float       m_fPitch;
};