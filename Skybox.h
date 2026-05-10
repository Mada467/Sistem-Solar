#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Skybox
{
public:
    Skybox();
    ~Skybox();

    bool Load(LPDIRECT3DDEVICE9 pDevice,
        const char* front, const char* back,
        const char* left, const char* right,
        const char* top, const char* bottom);
    void Render(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 cameraPos);
    void Cleanup();

private:
    struct SkyboxVertex
    {
        D3DXVECTOR3 position;
        D3DXVECTOR2 uv;
        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };

    void CreateFace(LPDIRECT3DDEVICE9 pDevice, int index,
        D3DXVECTOR3 v0, D3DXVECTOR3 v1,
        D3DXVECTOR3 v2, D3DXVECTOR3 v3);

    LPDIRECT3DVERTEXBUFFER9 m_pVB[6];
    LPDIRECT3DTEXTURE9      m_pTextures[6];
    float                   m_fSize;
};