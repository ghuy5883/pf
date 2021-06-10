#pragma once
#include "Graphics.h"
#include <d3d11.h>
#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer(Graphics* graphics,
		const void* vertData, uint32_t vertCount, uint32_t vertStride,
		const void* indexData, uint32_t indexCount, uint32_t indexStride
	);
	~VertexBuffer();
	void Draw() const;

private:
	Graphics* m_Graphics;
	ID3D11Buffer* m_Buffer;
	ID3D11Buffer* m_IndexBuffer;
	//Number of bytes per vertex
	uint32_t m_VertStride;
	//Number of vertices
	uint32_t m_VertCount;
	DXGI_FORMAT m_IndexFormat;
	uint32_t m_IndexCount;
	uint32_t m_IndexStride;
};

