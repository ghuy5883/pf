#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics* graphics, const void* vertData, uint32_t vertCount, uint32_t vertStride, const void* indexData, uint32_t indexCount, uint32_t indexStride)
{
	m_Graphics = graphics;
	m_VertCount = vertCount;
	m_VertStride = vertStride;
	m_IndexCount = indexCount;
	m_IndexStride = indexStride;
	if (m_IndexStride == 2) {
		m_IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else {
		m_IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}
	//Instantiate the buffer with an external function in the graphics class.
	m_Buffer = m_Graphics->CreateGraphicsBuffer(vertData, vertCount*vertStride, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	m_IndexBuffer = m_Graphics->CreateGraphicsBuffer(indexData, indexCount*indexStride, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

VertexBuffer::~VertexBuffer()
{
	m_Buffer->Release();
	m_IndexBuffer->Release();
}

void VertexBuffer::Draw() const
{
	//pOffset: offset, defaulted to 0.
	const UINT p_Offset = 0;
	m_Graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Buffer, &m_VertStride, &p_Offset);
	m_Graphics->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, m_IndexFormat, p_Offset);
	//Draw the image.
	m_Graphics->GetDeviceContext()->DrawIndexed((m_IndexCount), 0, 0);

}
