#include "Planet.h"

Planet::Planet()
{
    m_pMesh = NULL;
    m_pMaterials = NULL;
    m_pTextures = NULL;
    m_dwNumMaterials = 0;

    m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_OrbitCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fScale = 1.0f;
    m_fRotationSpeed = 0.0f;
    m_fRotationAngle = 0.0f;
    m_fOrbitSpeed = 0.0f;
    m_fOrbitAngle = 0.0f;
    m_fOrbitRadius = 0.0f;
}

Planet::~Planet()
{
    Cleanup();
}

bool Planet::Load(LPDIRECT3DDEVICE9 pDevice, const char* meshFile, const char* textureFile)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    if (FAILED(D3DXLoadMeshFromX(
        meshFile,
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &pD3DXMtrlBuffer,
        NULL,
        &m_dwNumMaterials,
        &m_pMesh)))
    {
        return false;
    }

    LPD3DXMESH pTempMesh;
    if (SUCCEEDED(m_pMesh->CloneMeshFVF(
        D3DXMESH_MANAGED,
        D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
        pDevice,
        &pTempMesh)))
    {
        m_pMesh->Release();
        m_pMesh = pTempMesh;
        D3DXComputeNormals(m_pMesh, NULL);
    }

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        m_pMaterials[i] = d3dxMaterials[i].MatD3D;
        m_pMaterials[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_pMaterials[i].Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_pTextures[i] = NULL;

        if (textureFile != NULL)
        {
            D3DXCreateTextureFromFileEx(
                pDevice,
                textureFile,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                0,
                D3DFMT_UNKNOWN,
                D3DPOOL_MANAGED,
                D3DX_FILTER_LINEAR,
                D3DX_FILTER_LINEAR,
                0, NULL, NULL,
                &m_pTextures[i]);
        }
        else if (d3dxMaterials[i].pTextureFilename != NULL)
        {
            D3DXCreateTextureFromFileEx(
                pDevice,
                d3dxMaterials[i].pTextureFilename,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                0,
                D3DFMT_UNKNOWN,
                D3DPOOL_MANAGED,
                D3DX_FILTER_LINEAR,
                D3DX_FILTER_LINEAR,
                0, NULL, NULL,
                &m_pTextures[i]);
        }
    }

    pD3DXMtrlBuffer->Release();
    return true;
}

void Planet::Update(float deltaTime)
{
    m_fRotationAngle += m_fRotationSpeed * deltaTime;
    m_fOrbitAngle += m_fOrbitSpeed * deltaTime;

    if (m_fOrbitRadius > 0.0f)
    {
        m_Position.x = m_OrbitCenter.x + m_fOrbitRadius * cosf(m_fOrbitAngle);
        m_Position.y = m_OrbitCenter.y;
        m_Position.z = m_OrbitCenter.z + m_fOrbitRadius * sinf(m_fOrbitAngle);
    }
}

void Planet::Render(LPDIRECT3DDEVICE9 pDevice)
{
    D3DXMATRIX matScale, matRotation, matTranslation, matWorld;

    D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
    D3DXMatrixRotationY(&matRotation, m_fRotationAngle);
    D3DXMatrixTranslation(&matTranslation, m_Position.x, m_Position.y, m_Position.z);

    matWorld = matScale * matRotation * matTranslation;
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);

    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        pDevice->SetMaterial(&m_pMaterials[i]);
        pDevice->SetTexture(0, m_pTextures[i]);
        m_pMesh->DrawSubset(i);
    }

    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Planet::Cleanup()
{
    if (m_pTextures)
    {
        for (DWORD i = 0; i < m_dwNumMaterials; i++)
            if (m_pTextures[i])
                m_pTextures[i]->Release();
        delete[] m_pTextures;
        m_pTextures = NULL;
    }

    if (m_pMaterials)
    {
        delete[] m_pMaterials;
        m_pMaterials = NULL;
    }

    if (m_pMesh)
    {
        m_pMesh->Release();
        m_pMesh = NULL;
    }
}

void Planet::SetPosition(float x, float y, float z)
{
    m_Position = D3DXVECTOR3(x, y, z);
}

void Planet::SetScale(float scale)
{
    m_fScale = scale;
}

void Planet::SetRotationSpeed(float speed)
{
    m_fRotationSpeed = speed;
}

void Planet::SetOrbitSpeed(float speed)
{
    m_fOrbitSpeed = speed;
}

void Planet::SetOrbitRadius(float radius)
{
    m_fOrbitRadius = radius;
}

void Planet::SetOrbitCenter(float x, float y, float z)
{
    m_OrbitCenter = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Planet::GetPosition()
{
    return m_Position;
}