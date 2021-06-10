#pragma once

#pragma warning(disable : 6387)

class Graphics
{
public:
    enum TextureSlot
    {
        TEXTURE_SLOT_DIFFUSE,
        TEXTURE_SLOT_NORMAL,
		TEXTURE_SLOT_TOTAL
    };

	enum ConstantBuffer
	{
		CONSTANT_BUFFER_CAMERA,
		CONSTANT_BUFFER_RENDEROBJ,
        CONSTANT_BUFFER_LIGHTING,
		CONSTANT_BUFFER_SKINNING,
	};

    class Color4
    {
    public:
        Color4()
            : r(0), g(0), b(0), a(0)
        {}
        Color4(float _r, float _g, float _b, float _a = 1.0f)
            : r(_r), g(_g), b(_b), a(_a)
        {}
        float r, g, b, a;
    };

    Graphics();
    ~Graphics();

    static Graphics* Get() { return s_theGraphics; }

    void InitD3D(HWND hWnd, float width, float height);
    void BeginFrame(const Color4 &clearColor);
    void EndFrame();
    void CleanD3D();

    // Getters for member variables.
	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

    IDXGISwapChain* GetSwapChain() { return pSwapChain; }
    ID3D11Device* Get3dDevice() { return pd3dDevice; }
    ID3D11DeviceContext* GetDeviceContext() { return pImmediateContext; }

    ID3D11DepthStencilView* GetDepthStencilView() { return mBackBufferStencilView; }

    ID3D11RenderTargetView* GetBackBuffer() { return mBackBuffer; }
    ID3D11RenderTargetView* GetRenderTexture() { return mRenderTextureView; }
    ID3D11RenderTargetView* GetRenderTextureHalf() { return mRenderTextureHalf; }
    ID3D11RenderTargetView* GetRenderTextureQuarter() { return mRenderTextureQuarter; }
    ID3D11RenderTargetView* GetRenderTextureQuarter2() { return mRenderTextureQuarter2; }

    ID3D11BlendState* GetBlendAdditive() { return mBlendAdditive; }
    ID3D11BlendState* GetBlendOpaque() { return mBlendOpaque; }

    // Set the render texture view member variable.
    void SetRenderTextureView(ID3D11RenderTargetView* pRender) { mRenderTextureView = pRender; }
    void SetRenderTextureViewHalf(ID3D11RenderTargetView* pRender) { mRenderTextureHalf = pRender; }
    void SetRenderTextureViewQuarter(ID3D11RenderTargetView* pRender) { mRenderTextureQuarter = pRender; }
    void SetRenderTextureViewQuarter2(ID3D11RenderTargetView* pRender) { mRenderTextureQuarter2 = pRender; }

    // Helper to simplify process of updating buffers.
    void UploadBuffer(ID3D11Buffer* buffer, const void* data, size_t dataSize);
    ID3D11Buffer* CreateGraphicsBuffer(const void* initialData, int inDataSize,
        D3D11_BIND_FLAG inBindFlags, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, 
        D3D11_USAGE inUsage);

    // Creates the depth stencil.
    bool CreateDepthStencil(int inWidth, int inHeight,
        ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView);

    // Create the Depth/Stencil state.
    ID3D11DepthStencilState* CreateDepthStencilState(
        bool inDepthTestEnable, D3D11_COMPARISON_FUNC inDepthComparisonFunction);

    // Create blend state.
    ID3D11BlendState* CreateBlendState(bool enable, D3D11_BLEND srcBlend, D3D11_BLEND dstBlend);

    // Set blend state.
    void SetBlendState(ID3D11BlendState* inBlendState);

    // Set blend state member variables.
    void SetBlendOpaque(ID3D11BlendState* pBlend) { mBlendOpaque = pBlend; }
    void SetBlendAdditive(ID3D11BlendState* pBlend) { mBlendAdditive = pBlend; }

    // Set the texture.
    void SetActiveTexture(int slot, ID3D11ShaderResourceView* pView);
    void SetActiveSampler(int slot, ID3D11SamplerState* pSampler);

    // Set render target.
    void SetRenderTarget(ID3D11RenderTargetView* renderTarget,
        ID3D11DepthStencilView* depthView);

    // Set viewport.
    void SetViewport(float x, float y, float width, float height);


private:
    static Graphics* s_theGraphics;
    float mScreenWidth;
    float mScreenHeight;

    IDXGISwapChain* pSwapChain;
    ID3D11Device* pd3dDevice;
    ID3D11DeviceContext* pImmediateContext;

    ID3D11RenderTargetView* pRenderTargetView;
    ID3D11RenderTargetView* mRenderTextureView;
    ID3D11RenderTargetView* mRenderTextureHalf;
    ID3D11RenderTargetView* mRenderTextureQuarter;
    ID3D11RenderTargetView* mRenderTextureQuarter2;
    ID3D11RenderTargetView* mBackBuffer;

    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11DepthStencilView* mBackBufferStencilView;

    ID3D11Texture2D* mDepthTexture;

    ID3D11DepthStencilState* mDepthStencilState;

    ID3D11SamplerState* mSamplerState;

    ID3D11BlendState* mBlendOpaque;
    ID3D11BlendState* mBlendAdditive;
};