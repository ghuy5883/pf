#include "stdafx.h"
#include "RenderCube.h"

struct VertexPosColor
{
	Vector3 pos;
	Graphics::Color4 color;
	Vector2 uv;
	Vector3 normal;
};

RenderCube::RenderCube(AssetManager* pAssetManager, const Shader* pShader, const
	Texture* pTex)
	: RenderObj(pAssetManager->GetGraphics(), nullptr)
{
	static VertexPosColor cubeVertex[] =
	{
		//Back face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0F, 0.0F, -1.0F) },
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0F, 0.0F, -1.0F) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0F, 0.0F, -1.0F) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f),Vector3(0.0F, 0.0F, -1.0F) },
		//Front face
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.1f, 0.8f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0F, 0.0F, 1.0F) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.1f, 0.8f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0F, 0.0F, 1.0F) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.1f, 0.8f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0F, 0.0F, 1.0F)  },
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.1f, 0.8f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0F, 0.0F, 1.0F)  },
		//Right face
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.9f, 0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0F, 0.0F, 0.0F)  },
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(0.9f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0F, 0.0F, 0.0F) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(0.9f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0F, 0.0F, 0.0F) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.9f, 0.5f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0F, 0.0F, 0.0F) },
		//Left face
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.9f, 0.0f, 0.6f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0F, 0.0F, 0.0F) },
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(0.9f, 0.0f, 0.6f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0F, 0.0F, 0.0F) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(0.9f, 0.0f, 0.6f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0F, 0.0F, 0.0F) },
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.9f, 0.0f, 0.6f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0F, 0.0F, 0.0F) },
		//Top face
	{ Vector3(-0.5f, 0.5f, -0.5f), Graphics::Color4(0.7f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0F, 1.0F, 0.0F) },
	{ Vector3(0.5f, 0.5f, -0.5f), Graphics::Color4(0.7f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0F, 1.0F, 0.0F) },
	{ Vector3(0.5f, 0.5f, 0.5f), Graphics::Color4(0.7f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0F, 1.0F, 0.0F) },
	{ Vector3(-0.5f, 0.5f, 0.5f), Graphics::Color4(0.7f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0F, 1.0F, 0.0F) },
		//Bottom face
	{ Vector3(-0.5f, -0.5f, -0.5f), Graphics::Color4(0.6f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0F, -1.0F, 0.0F) },
	{ Vector3(0.5f, -0.5f, -0.5f), Graphics::Color4(0.6f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0F, -1.0F, 0.0F) },
	{ Vector3(0.5f, -0.5f, 0.5f), Graphics::Color4(0.6f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0F, -1.0F, 0.0F) },
	{ Vector3(-0.5f, -0.5f, 0.5f), Graphics::Color4(0.6f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0F, -1.0F, 0.0F) },
	};

	static uint16_t cubeIndex[] =
	{
	2, 1, 0,
	3, 2, 0,
	6, 4, 5,
	7, 4, 6,
	10, 8, 9,
	11, 8, 10,
	14, 13, 12,
	15, 14, 12,
	18, 16, 17,
	19, 16, 18,
	22, 21, 20,
	23,	22, 20
	};

	m_VertexBuffer = new VertexBuffer(m_Graphics, cubeVertex, sizeof(cubeVertex), sizeof(VertexPosColor), cubeIndex, sizeof(cubeIndex), sizeof(uint16_t));

	// Create mesh
	m_Mesh = new Mesh(pAssetManager, m_VertexBuffer, pShader);

	// Add texture
	m_Mesh->SetTexture(Graphics::TEXTURE_SLOT_DIFFUSE, pTex);
}

RenderCube::~RenderCube()
{
	delete m_Mesh;
}

void RenderCube::Draw()
{
	/* -------------------------------------------------------------------------------- */
		//Render a frame.
	/* -------------------------------------------------------------------------------- */

	//Update object buffer with object data and upload to GPU.
	m_Graphics->UploadBuffer(m_ObjectBuffer, &m_ModelToWorldObj, sizeof(m_ModelToWorldObj));

	//Bind the object buffer;
	m_Graphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_RENDEROBJ, 1, &m_ObjectBuffer);

	//Draw the vertex buffer
	m_Mesh->Draw();
}
