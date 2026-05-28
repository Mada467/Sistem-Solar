#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Planet
{
public:
    Planet();
    ~Planet();

    bool Load(LPDIRECT3DDEVICE9 pDevice, const char* meshFile, const char* textureFile);
    void Update(float deltaTime);
    void Render(LPDIRECT3DDEVICE9 pDevice);
    void Cleanup();

    void SetPosition(float x, float y, float z);
    void SetScale(float scale);
    void SetRotationSpeed(float speed);
    void SetOrbitSpeed(float speed);
    void SetOrbitRadius(float radius);
    void SetOrbitCenter(float x, float y, float z);

    void AddRotationX(float angle);
    void AddRotationY(float angle);
    void AddRotationZ(float angle);

    void Translate(float dx, float dy, float dz);

    D3DXVECTOR3 GetPosition();

private:
    LPD3DXMESH          m_pMesh;
    D3DMATERIAL9* m_pMaterials;
    LPDIRECT3DTEXTURE9* m_pTextures;
    DWORD               m_dwNumMaterials;

    D3DXVECTOR3 m_Position;
    D3DXVECTOR3 m_OrbitCenter;
    float       m_fScale;
    float       m_fRotationSpeed;
    float       m_fRotationAngle;
    float       m_fRotationAngleX;
    float       m_fRotationAngleZ;
    float       m_fOrbitSpeed;
    float       m_fOrbitAngle;
    float       m_fOrbitRadius;
};