#include "RenderObj.h"
#include "stdafx.h"
#include "../Source/game.h"


RenderObj::RenderObj(Graphics* graphics, const Mesh* pMesh, Game* game)
{
	//Use arguments to initialize member variables.
	m_Mesh = pMesh;
	m_Graphics = graphics;
	m_Game = game;

	//Initialize the object buffer pointer.
	m_ObjectBuffer = m_Graphics->CreateGraphicsBuffer(&m_ModelToWorldObj, sizeof(m_ModelToWorldObj), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

RenderObj::~RenderObj()
{
	m_ObjectBuffer->Release();
	/* No need to delete the object mesh; the asset manager will handle that. Just release object buffer. */
	for (int i = 0; i < m_Components.size(); i++) {
		delete m_Components[i];
	}
}

void RenderObj::Draw()
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


void RenderObj::AddComponent(Component* pComp)
{
	m_Components.push_back(pComp);
}

void RenderObj::Update(float deltaTime)
{
	for (int i = 0; i < m_Components.size(); i++) {
		m_Components[i]->Update(deltaTime);
	}
}

Game* RenderObj::GetGame()
{
	return m_Game;
}
