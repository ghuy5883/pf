#include "stdafx.h"
#include "assetManager.h"
#include "Graphics.h"

AssetManager::AssetManager(Graphics* pGraphics)
    : mGraphics(pGraphics)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::Clear()
{
    m_ShaderCache.Clear();
    m_MeshCache.Clear();
    m_TextureCache.Clear();
    m_SkeletonCache.Clear();
    m_AnimationCache.Clear();
}

Shader* AssetManager::GetShader(const std::wstring& shaderName)
{
    return m_ShaderCache.Get(shaderName);
}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader)
{
    m_ShaderCache.Cache(shaderName, pShader);
}

Texture* AssetManager::LoadTexture(const std::wstring& fileName)
{
   return  m_TextureCache.Load(fileName);
}

Mesh* AssetManager::GetMesh(const std::wstring& meshName)
{
    return m_MeshCache.Get(meshName);
}

void AssetManager::SetMesh(const std::wstring& meshName, Mesh* pMesh)
{
    m_MeshCache.Cache(meshName, pMesh);
}

Mesh* AssetManager::LoadMesh(const std::wstring& fileName)
{
    Mesh* pMesh = m_MeshCache.Load(fileName);
    return pMesh;
}

Skeleton* AssetManager::LoadSkeleton(const std::wstring& fileName)
{
    Skeleton* pSkeleton = m_SkeletonCache.Load(fileName);
    return pSkeleton;
}

Animation* AssetManager::LoadAnimation(const std::wstring& fileName)
{
    Animation* pAnimation = m_AnimationCache.Load(fileName);
    return pAnimation;
}
