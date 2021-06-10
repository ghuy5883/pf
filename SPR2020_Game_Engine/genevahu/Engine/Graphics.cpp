#include "Graphics.h"
#include "Graphics.h"
#include "Graphics.h"
#include "stdafx.h"
#include "Graphics.h"
#include "Shader.h"
#include "engineMath.h"

#pragma comment (lib, "d3d11.lib") 

/*static*/ Graphics* Graphics::s_theGraphics = nullptr;

struct VertexPosColor
{
    Vector3 pos;
    Graphics::Color4 color;
};

Graphics::Graphics()
    : mScreenWidth(0)
    , mScreenHeight(0)
{
    DbgAssert(nullptr == s_theGraphics, "You can only have 1 Graphics");
    s_theGraphics = this;

    pSwapChain = nullptr;
    pd3dDevice = nullptr;
    pImmediateContext = nullptr;
    pRenderTargetView = nullptr;
    mDepthStencilState = nullptr;
    mDepthTexture = nullptr;
    mDepthStencilView = nullptr;
    mSamplerState = nullptr;
}

Graphics::~Graphics()
{
    DbgAssert(this == s_theGraphics, "There can only be 1 Graphics");
    s_theGraphics = nullptr;
}

void Graphics::InitD3D(HWND hWnd, float width, float height)
{
    /* -------------------------------------------------------------------------------- */
        //Set the window width and height.
    /* -------------------------------------------------------------------------------- */
    mScreenWidth = width;
    mScreenHeight = height;

    /* -------------------------------------------------------------------------------- */   
        //Create a device and immediate context; necessary to render a scene.
    /* -------------------------------------------------------------------------------- */
    //Fill in the DXGI_SWAP_CHAIN_DESC structure
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    //Initialize the device and swap chain at the same time
    D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
    UINT               numLevelsRequested = 1;
    D3D_FEATURE_LEVEL  FeatureLevelsSupported;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
//#ifdef _DEBUG
        D3D11_CREATE_DEVICE_DEBUG,
//#else
       // 0,
//#endif
        & FeatureLevelsRequested,
        numLevelsRequested,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pd3dDevice,
        &FeatureLevelsSupported,
        &pImmediateContext);

    /* -------------------------------------------------------------------------------- */
        //Create a viewport and adjust settings
    /* -------------------------------------------------------------------------------- */
    D3D11_VIEWPORT Viewport = {
        0.0f,
        0.0f,
        width,
        height,
        0.0f,
        1.0f
    };

    pImmediateContext->RSSetViewports(1, &Viewport);
    //Fill memory block with zeroes to init it
    ZeroMemory(&Viewport, sizeof(Viewport));

    //Initialize some of the values
    Viewport.Width = width;
    Viewport.Height = height;
    Viewport.MaxDepth = 1.0f;
    Viewport.MinDepth = 0.0f;

    //Get a pointer to the back buffer
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (LPVOID*)&pBackBuffer);

    //Create a render-target view
    pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
        &mBackBuffer);

    pRenderTargetView = mBackBuffer;

    //Release the pointer to the back buffer; don't need it anymore
    pBackBuffer->Release();

    //Set the primitives to "triangle list"
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    /* -------------------------------------------------------------------------------- */
        //Initialize the 2D texture, the depth stencil view, and the depth stencil state.
    /* -------------------------------------------------------------------------------- */
    mDepthStencilState = CreateDepthStencilState(true, D3D11_COMPARISON_LESS);
    CreateDepthStencil(static_cast<int> (width), static_cast<int> (height), &mDepthTexture, &mBackBufferStencilView); 
    mDepthStencilView = mBackBufferStencilView;

    /* -------------------------------------------------------------------------------- */
    // Initialize the sampler state.
    /* -------------------------------------------------------------------------------- */
    D3D11_SAMPLER_DESC sDesc;
    ZeroMemory(&sDesc, sizeof(sDesc));
    sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.MinLOD = (-FLT_MAX);
    sDesc.MaxLOD = (FLT_MAX);
    sDesc.MipLODBias = 0.0f;
    sDesc.MaxAnisotropy = 1;
    sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sDesc.BorderColor[0] = 1.0f;
    sDesc.BorderColor[1] = 1.0f;
    sDesc.BorderColor[2] = 1.0f;
    sDesc.BorderColor[3] = 1.0f;

    pd3dDevice->CreateSamplerState(&sDesc, &mSamplerState);
    SetActiveSampler(0, mSamplerState);


    /* -------------------------------------------------------------------------------- */
        // Change forward facing winding order from CW to CCW.
    /* -------------------------------------------------------------------------------- */
    // Rasterizer description
    D3D11_RASTERIZER_DESC rDesc;
    ZeroMemory(&rDesc, sizeof(rDesc));
    rDesc.FillMode = D3D11_FILL_SOLID;
    rDesc.CullMode = D3D11_CULL_BACK;
    rDesc.DepthBias = 0;
    rDesc.SlopeScaledDepthBias = 0.0f;
    rDesc.DepthBiasClamp = 0.0f;
    rDesc.DepthClipEnable = true;
    rDesc.ScissorEnable = false;
    rDesc.MultisampleEnable = false;
    rDesc.AntialiasedLineEnable = false;
    rDesc.FrontCounterClockwise = true;

    ID3D11RasterizerState* pRasterize = nullptr;

    pd3dDevice->CreateRasterizerState(&rDesc, &pRasterize);
    pImmediateContext->RSSetState(pRasterize);

    // Can immediately release the rasterizer pointer
    pRasterize->Release();
}

void Graphics::BeginFrame(const Color4 &clearColor)
{
    // Clear the screen with the selected color.
    const FLOAT* ColorRGBA = reinterpret_cast<const FLOAT*>(&clearColor);
    pImmediateContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
    // Set depth stencil state.
    pImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
    // Clear depth buffer to 1.0.
    pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    // Bind the view, set the depth stencil view to be active.
    pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, mDepthStencilView);
}

void Graphics::EndFrame()
{
    /* -------------------------------------------------------------------------------- */
        //Swap buffers and show display
    /* -------------------------------------------------------------------------------- */
    pSwapChain->Present(1, 0);
}

void Graphics::CleanD3D()
{
    /* -------------------------------------------------------------------------------- */
        //Free the D3D resources.
    /* -------------------------------------------------------------------------------- */
    pSwapChain->Release();
    pd3dDevice->Release();
    pImmediateContext->Release();

    mBackBuffer->Release();
    mRenderTextureView->Release();
    mRenderTextureHalf->Release();
    mRenderTextureQuarter->Release();
    mRenderTextureQuarter2->Release();

    mDepthStencilState->Release();

    mBackBufferStencilView->Release();

    mDepthTexture->Release();
    mSamplerState->Release();

    mBlendAdditive->Release();
    mBlendOpaque->Release();
}

void Graphics::UploadBuffer(ID3D11Buffer* buffer, const void* data, size_t dataSize) {
    /* -------------------------------------------------------------------------------- */
        // Upload the buffer
    /* -------------------------------------------------------------------------------- */
    //Create a mapped subresource to hold new data.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    //Disable GPU access to the vertex buffer data.
    pImmediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    //Update the vertex buffer here.
    memcpy(mappedResource.pData, data, dataSize);
    //Reenable GPU access to the vertex buffer data.
    pImmediateContext->Unmap(buffer, 0);
}

ID3D11Buffer* Graphics::CreateGraphicsBuffer(const void* initialData, int inDataSize, D3D11_BIND_FLAG inBindFlags, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage)
{
    /* -------------------------------------------------------------------------------- */
        // Create a buffer and return it
    /* -------------------------------------------------------------------------------- */
    ID3D11Buffer* m_Buffer;
    ///Write the buffer description.
    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.ByteWidth = inDataSize;
    vertexBufferDesc.Usage = inUsage;
    vertexBufferDesc.BindFlags = inBindFlags;
    vertexBufferDesc.CPUAccessFlags = inCPUAccessFlags;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    //Create the buffer that holds the drawing info.
    pd3dDevice->CreateBuffer(&vertexBufferDesc, nullptr, &m_Buffer);

    //If the data pointer passed in isn't null, use the data with the buffer and return buffer pointer
    if (initialData != nullptr) {
        UploadBuffer(m_Buffer, initialData, inDataSize);
        return m_Buffer;
    }
    
    return nullptr;
}

bool Graphics::CreateDepthStencil(int inWidth, int inHeight, ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView)
{
    /* -------------------------------------------------------------------------------- */
        // Create the depth buffer
    /* -------------------------------------------------------------------------------- */
    
    // Write the description for the 2D texture 
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = inWidth;
    descDepth.Height = inHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    HRESULT hr = pd3dDevice->CreateTexture2D(&descDepth, NULL, pDepthTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Flags = 0;
    descDSV.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    hr = pd3dDevice->CreateDepthStencilView(mDepthTexture, // Depth stencil texture
        &descDSV, // Depth stencil desc
        pDepthView);  // [out] Depth stencil view

    return !(hr == S_OK);
}

ID3D11DepthStencilState* Graphics::CreateDepthStencilState(bool inDepthTestEnable, D3D11_COMPARISON_FUNC inDepthComparisonFunction)
{
    // Write the description for the depth stencil buffer
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    // Depth test parameters
    dsDesc.DepthEnable = inDepthTestEnable;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = inDepthComparisonFunction;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    ID3D11DepthStencilState* pDSState;
    pd3dDevice->CreateDepthStencilState(&dsDesc, &pDSState);

    return pDSState;
}

ID3D11BlendState* Graphics::CreateBlendState(bool enable, D3D11_BLEND srcBlend, D3D11_BLEND dstBlend)
{
    D3D11_BLEND_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = false;
    desc.RenderTarget[0].BlendEnable = enable;
    desc.RenderTarget[0].SrcBlend = srcBlend;
    desc.RenderTarget[0].DestBlend = dstBlend;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* pBlend;
    pd3dDevice->CreateBlendState(&desc, &pBlend);
    return pBlend;
}

void Graphics::SetBlendState(ID3D11BlendState* inBlendState)
{
    float blendFactor[4];
    // Setup the blend factor.
    blendFactor[0] = 0.0f;
    blendFactor[1] = 0.0f;
    blendFactor[2] = 0.0f;
    blendFactor[3] = 0.0f;
    pImmediateContext->OMSetBlendState(inBlendState, blendFactor, 0xffffffff);

}

void Graphics::SetActiveTexture(int slot, ID3D11ShaderResourceView* pView)
{
    // Set the texture as the shader resource.
    pImmediateContext->PSSetShaderResources(slot, 1, &pView);
}

void Graphics::SetActiveSampler(int slot, ID3D11SamplerState* pSampler)
{
    // Set the sampler.
    pImmediateContext->PSSetSamplers(slot, 1, &pSampler);
}

void Graphics::SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthView)
{
    pRenderTargetView = renderTarget;
    mDepthStencilView = depthView;
    pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, mDepthStencilView);
}

void Graphics::SetViewport(float x, float y, float width, float height)
{
    D3D11_VIEWPORT Viewport;
    //Fill memory block with zeroes before init it
    ZeroMemory(&Viewport, sizeof(Viewport));
    Viewport.TopLeftX = x;
    Viewport.TopLeftY = y;
    Viewport.Width = width;
    Viewport.Height = height;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;

    // Set the viewport.
    pImmediateContext->RSSetViewports(1, &Viewport);
}
