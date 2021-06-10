#include "stdafx.h"
#include "texture.h"
#include "Graphics.h"
#include "DirectXTK\Inc\DDSTextureLoader.h"
#include "DirectXTK\Inc\WICTextureLoader.h"
#include <string>

Texture::Texture(Graphics *graphics)
    : mGraphics(graphics)
    , mResource(nullptr)
    , mView(nullptr)
    , mWidth(0)
    , mHeight(0)
{
}

Texture::~Texture()
{
    Free();
}

void Texture::Free()
{
    if (nullptr != mView)
    {
        mView->Release();
        mView = nullptr;
    }
    if (nullptr != mResource)
    {
        mResource->Release();
        mResource = nullptr;
    }
    mWidth = 0;
    mHeight = 0;
}

bool Texture::Load(const WCHAR* fileName)
{
    Free();     // in case there was already a texture loaded here, release it

    ID3D11Device *pDev = mGraphics->Get3dDevice();

    std::wstring fileStr(fileName);
    std::wstring extension = fileStr.substr(fileStr.find_last_of('.'));
    HRESULT hr = E_FAIL;
    if (extension == L".dds" || extension == L".DDS")
        hr = DirectX::CreateDDSTextureFromFile(pDev, fileName, &mResource, &mView);
    else
        hr = DirectX::CreateWICTextureFromFile(pDev, fileName, &mResource, &mView);
    DbgAssert(hr == S_OK, "Problem Creating Texture From File");
    if (S_OK != hr)
        return false;

    CD3D11_TEXTURE2D_DESC textureDesc;
    ((ID3D11Texture2D*)mResource)->GetDesc(&textureDesc);
    mWidth = textureDesc.Width;
    mHeight = textureDesc.Height;

    return true;
}

Texture* Texture::StaticLoad(const WCHAR* fileName, AssetManager* pManager)
{
    Texture* pTex = new Texture(Graphics::Get());
    if (false == pTex->Load(fileName))
    {
        delete pTex;
        return nullptr;
    }
    return pTex;
}

void Texture::SetActive(int slot) const
{
	mGraphics->SetActiveTexture(slot, mView);
}

ID3D11RenderTargetView* Texture::CreateRenderTarget(int inWidth, int inHeight, DXGI_FORMAT format)
{
    DXGI_SAMPLE_DESC sampleDesc;
    sampleDesc.Count = 1;
    sampleDesc.Quality = 0;

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = inWidth;
    desc.Height = inHeight;
    desc.MipLevels = desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc = sampleDesc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;

    ID3D11RenderTargetView* pRender;
    mGraphics->Get3dDevice()->CreateTexture2D(&desc, NULL, reinterpret_cast<ID3D11Texture2D**>(&mResource));
    mGraphics->Get3dDevice()->CreateShaderResourceView(mResource, NULL, &mView);
    mGraphics->Get3dDevice()->CreateRenderTargetView(mResource, NULL, &pRender);

    return pRender;
}

