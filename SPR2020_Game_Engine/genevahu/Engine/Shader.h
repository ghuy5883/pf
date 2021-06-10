#pragma once

class Graphics;

class Shader
{
public:
    Shader(Graphics* graphics);
    ~Shader();

    bool Load(const WCHAR* fileName, const D3D11_INPUT_ELEMENT_DESC* layoutArray, int numLayoutElements);
    void SetActive() const;

private:
    Graphics* mGraphics;
	// TODO Lab 02g
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_inputLayout;
};