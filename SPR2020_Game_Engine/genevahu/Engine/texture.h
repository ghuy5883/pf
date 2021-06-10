#pragma once

class AssetManager;
class Graphics;

class Texture
{
public:
    Texture(Graphics *graphics);
    ~Texture();
    
    void Free();
    bool Load(const WCHAR* fileName);
    static Texture* StaticLoad(const WCHAR* fileName, AssetManager* pManager);
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    void SetActive(int slot) const;
    void SetActiveTexture(int slot, ID3D11ShaderResourceView* pView);
    void SetActiveSampler(int slot, ID3D11SamplerState* pSampler);
    ID3D11RenderTargetView* CreateRenderTarget(int inWidth, int inHeight, DXGI_FORMAT format);


private:
    Graphics *mGraphics;
    ID3D11Resource *mResource;
    ID3D11ShaderResourceView *mView;
    D3D11_SUBRESOURCE_DATA* mSubResource;
    int mWidth;
    int mHeight;
};