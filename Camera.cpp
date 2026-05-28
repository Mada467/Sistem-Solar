#include "Camera.h"

Camera::Camera()
{
    m_Position = D3DXVECTOR3(0.0f, 5.0f, -20.0f);
    m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_fYaw = 0.0f;
    m_fPitch = 0.0f;
}

void Camera::SetPosition(float x, float y, float z)
{
    m_Position = D3DXVECTOR3(x, y, z);
}

void Camera::SetTarget(float x, float y, float z)
{
    m_Target = D3DXVECTOR3(x, y, z);
}

void Camera::MoveForward(float delta)
{
    D3DXVECTOR3 direction = m_Target - m_Position;
    D3DXVec3Normalize(&direction, &direction);
    m_Position += direction * delta;
    m_Target += direction * delta;
}

void Camera::MoveRight(float delta)
{
    D3DXVECTOR3 direction = m_Target - m_Position;
    D3DXVECTOR3 right;
    D3DXVec3Cross(&right, &direction, &m_Up);
    D3DXVec3Normalize(&right, &right);
    m_Position += right * delta;
    m_Target += right * delta;
}

void Camera::MoveUp(float delta)
{
    m_Position.y += delta;
    m_Target.y += delta;
}

void Camera::RotateYaw(float angle)
{
    m_fYaw += angle;
    D3DXMATRIX rotMatrix;
    D3DXMatrixRotationY(&rotMatrix, m_fYaw);
    D3DXVECTOR3 direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    D3DXVec3TransformCoord(&direction, &direction, &rotMatrix);
    m_Target = m_Position + direction;
}

void Camera::RotatePitch(float angle)
{
    m_fPitch += angle;
    if (m_fPitch > 1.5f) m_fPitch = 1.5f;
    if (m_fPitch < -1.5f) m_fPitch = -1.5f;

    D3DXMATRIX rotY, rotX;
    D3DXMatrixRotationY(&rotY, m_fYaw);
    D3DXMatrixRotationX(&rotX, m_fPitch);
    D3DXMATRIX rot = rotX * rotY;
    D3DXVECTOR3 direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    D3DXVec3TransformCoord(&direction, &direction, &rot);
    m_Target = m_Position + direction;
}

void Camera::Update(LPDIRECT3DDEVICE9 pDevice)
{
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, &m_Position, &m_Target, &m_Up);
    pDevice->SetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj,
        D3DXToRadian(45.0f),
        800.0f / 600.0f,
        0.1f,
        1000.0f);
    pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

D3DXVECTOR3 Camera::GetPosition()
{
    return m_Position;
}