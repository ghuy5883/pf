#pragma once
#include "Shader.h"
#include "texture.h"
#include "mesh.h"
#include "assetCache.h"
#include "Animation.h"

class Graphics;

class AssetManager
{
public:
    AssetManager(Graphics* pGraphics);
    ~AssetManager();

    void Clear();

    Graphics* GetGraphics() { return mGraphics; }

    // Functions
    Shader* GetShader(const std::wstring& shaderName);
    void SetShader(const std::wstring& shaderName, Shader* pShader);
    Texture* LoadTexture(const std::wstring& fileName);
    Mesh* GetMesh(const std::wstring& meshName);
    void SetMesh(const std::wstring& meshName, Mesh* pMesh);
    Mesh* LoadMesh(const std::wstring& fileName);
    Skeleton* LoadSkeleton(const std::wstring& fileName);
    Animation* LoadAnimation(const std::wstring& fileName);

private:
    Graphics* mGraphics;
    AssetCache<Shader> m_ShaderCache = AssetCache<Shader>(this);
    AssetCache<Texture> m_TextureCache = AssetCache<Texture>(this);
    AssetCache<Mesh> m_MeshCache = AssetCache<Mesh>(this);
    AssetCache<Skeleton> m_SkeletonCache = AssetCache<Skeleton>(this);
    AssetCache<Animation> m_AnimationCache = AssetCache<Animation>(this);
};
