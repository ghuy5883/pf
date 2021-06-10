#include "stdafx.h"
#include "Shader.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib") 

Shader::Shader(Graphics* graphics)
    : mGraphics(graphics)
{
    m_inputLayout = nullptr;
    m_pixelShader = nullptr;
    m_vertexShader = nullptr;
}

Shader::~Shader()
{
    // TODO Lab 02g
    //Release pointers once they are no longer needed.
    m_inputLayout->Release();
    m_vertexShader->Release();
    m_pixelShader->Release();
}

static bool LoadShader(const WCHAR* filename, const char* entryPoint, const char* model, ID3DBlob*& pBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, model,
        dwShaderFlags, 0, &pBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            static wchar_t szBuffer[4096];
            _snwprintf_s(szBuffer, 4096, _TRUNCATE,
                L"%hs",
                (char*)pErrorBlob->GetBufferPointer());
            OutputDebugString(szBuffer);
#ifdef _WINDOWS
            MessageBox(nullptr, szBuffer, L"Error", MB_OK);
#endif
            pErrorBlob->Release();
        }
        return false;
    }
    if (pErrorBlob)
    {
        pErrorBlob->Release();
    }

    return true;
}

bool Shader::Load(const WCHAR* fileName, const D3D11_INPUT_ELEMENT_DESC* layoutArray, int numLayoutElements)
{
	// TODO Lab 2g
    /* -------------------------------------------------------------------------------- */
        //Loading function for the shader.
    /* -------------------------------------------------------------------------------- */
    //Call the pre-made LoadShader helper function.
    ID3DBlob* vertexBlob = nullptr;
    ID3DBlob* pixelBlob = nullptr;
    LoadShader(fileName, "VS", "vs_4_0", vertexBlob);
    LoadShader(fileName, "PS", "ps_4_0", pixelBlob);

    //Initialize the vertex and pixel shader member variables.
    mGraphics->Get3dDevice()->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &m_vertexShader);
    mGraphics->Get3dDevice()->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &m_pixelShader);

    //Create input layout.
    HRESULT hr = mGraphics->Get3dDevice()->CreateInputLayout(layoutArray, numLayoutElements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &m_inputLayout);

    //Finished with the blob pointers, they can be released now.
    vertexBlob->Release();
    pixelBlob->Release();

    //DbgAssert(false, "Failed to load shader");
	return false;
}

void Shader::SetActive() const
{
    //Setting vertex, pixel shaders and the input layout
    mGraphics->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
    mGraphics->GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
    mGraphics->GetDeviceContext()->IASetInputLayout(m_inputLayout);

}