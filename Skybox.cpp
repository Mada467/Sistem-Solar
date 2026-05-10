#include "Skybox.h"

Skybox::Skybox()
{
    for (int i = 0; i < 6; i++)
    {
        m_pVB[i] = NULL;
        m_pTextures[i] = NULL;
    }
    m_fSize = 500.0f;
}

Skybox::~Skybox()
{
    Cleanup();
}

void Skybox::CreateFace(LPDIRECT3DDEVICE9 pDevice, int index,
    D3DXVECTOR3 v0, D3DXVECTOR3 v1,
    D3DXVECTOR3 v2, D3DXVECTOR3 v3)
{
    pDevice->CreateVertexBuffer(
        4 * sizeof(SkyboxVertex),
        0, SkyboxVertex::FVF,
        D3DPOOL_MANAGED,
        &m_pVB[index], NULL);

    SkyboxVertex* pVerts;
    m_pVB[index]->Lock(0, 0, (void**)&pVerts, 0);

    pVerts[0].position = v0; pVerts[0].uv = D3DXVECTOR2(0.0f, 1.0f);
    pVerts[1].position = v1; pVerts[1].uv = D3DXVECTOR2(0.0f, 0.0f);
    pVerts[2].position = v2; pVerts[2].uv = D3DXVECTOR2(1.0f, 1.0f);
    pVerts[3].position = v3; pVerts[3].uv = D3DXVECTOR2(1.0f, 0.0f);

    m_pVB[index]->Unlock();
}

bool Skybox::Load(LPDIRECT3DDEVICE9 pDevice,
    const char* front, const char* back,
    const char* left, const char* right,
    const char* top, const char* bottom)
{
    float s = m_fSize;

    // Front
    CreateFace(pDevice, 0,
        D3DXVECTOR3(-s, -s, s), D3DXVECTOR3(-s, s, s),
        D3DXVECTOR3(s, -s, s), D3DXVECTOR3(s, s, s));

    // Back
    CreateFace(pDevice, 1,
        D3DXVECTOR3(s, -s, -s), D3DXVECTOR3(s, s, -s),
        D3DXVECTOR3(-s, -s, -s), D3DXVECTOR3(-s, s, -s));

    // Left
    CreateFace(pDevice, 2,
        D3DXVECTOR3(-s, -s, -s), D3DXVECTOR3(-s, s, -s),
        D3DXVECTOR3(-s, -s, s), D3DXVECTOR3(-s, s, s));

    // Right
    CreateFace(pDevice, 3,
        D3DXVECTOR3(s, -s, s), D3DXVECTOR3(s, s, s),
        D3DXVECTOR3(s, -s, -s), D3DXVECTOR3(s, s, -s));

    // Top
    CreateFace(pDevice, 4,
        D3DXVECTOR3(-s, s, s), D3DXVECTOR3(-s, s, -s),
        D3DXVECTOR3(s, s, s), D3DXVECTOR3(s, s, -s));

    // Bottom
    CreateFace(pDevice, 5,
        D3DXVECTOR3(-s, -s, -s), D3DXVECTOR3(-s, -s, s),
        D3DXVECTOR3(s, -s, -s), D3DXVECTOR3(s, -s, s));

    // Incarca texturile cu mipmapping
    const char* files[6] = { front, back, left, right, top, bottom };
    for (int i = 0; i < 6; i++)
    {
        if (FAILED(D3DXCreateTextureFromFileEx(
            pDevice, files[i],
            D3DX_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_FILTER_LINEAR,
            D3DX_FILTER_LINEAR,
            0, NULL, NULL,
            &m_pTextures[i])))
        {
            return false;
        }
    }

    return true;
}

void Skybox::Render(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 cameraPos)
{
    D3DXMATRIX matWorld;
    D3DXMatrixTranslation(&matWorld, cameraPos.x, cameraPos.y, cameraPos.z);
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);

    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    pDevice->SetFVF(SkyboxVertex::FVF);

    for (int i = 0; i < 6; i++)
    {
        pDevice->SetTexture(0, m_pTextures[i]);
        pDevice->SetStreamSource(0, m_pVB[i], 0, sizeof(SkyboxVertex));
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    }

    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Skybox::Cleanup()
{
    for (int i = 0; i < 6; i++)
    {
        if (m_pTextures[i])
        {
            m_pTextures[i]->Release();
            m_pTextures[i] = NULL;
        }
        if (m_pVB[i])
        {
            m_pVB[i]->Release();
            m_pVB[i] = NULL;
        }
    }
}