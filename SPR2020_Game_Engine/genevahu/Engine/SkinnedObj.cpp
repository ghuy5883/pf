#include "stdafx.h"
#include "SkinnedObj.h"

SkinnedObj::SkinnedObj(Graphics* graphics,
	const Mesh* pMesh,
	Game* game):
	RenderObj(graphics, pMesh, game)
{
	// Fill all skinning matrices to the identity by default
	for (int i = 0; i < MAX_SKELETON_BONES; i++) {
		m_SkinConstants.c_skinMatrix[i] = Matrix4::Identity;
	}

	//Initialize the skin matrix buffer pointer.
	m_SkinningMatrixBuffer = m_Graphics->CreateGraphicsBuffer(&m_SkinConstants, sizeof(m_SkinConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

}

SkinnedObj::~SkinnedObj()
{
	m_SkinningMatrixBuffer->Release();
}

void SkinnedObj::Draw()
{
	/* -------------------------------------------------------------------------------- */
		//Render a frame.
	/* -------------------------------------------------------------------------------- */
	//Update object buffer with object data and upload to GPU.
	m_Graphics->UploadBuffer(m_ObjectBuffer, &m_ModelToWorldObj, sizeof(m_ModelToWorldObj));

	//Bind the object buffer;
	m_Graphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_RENDEROBJ, 1, &m_ObjectBuffer);

	// Update the skinning buffer
	m_Graphics->UploadBuffer(m_SkinningMatrixBuffer, &m_SkinConstants, sizeof(m_SkinConstants));

	// Upload the skinning matrices.
	m_Graphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_SKINNING, 1, &m_SkinningMatrixBuffer);

	//Draw the vertex buffer
	m_Mesh->Draw();
}
