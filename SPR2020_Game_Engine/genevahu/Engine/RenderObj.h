#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "engineMath.h"
#include "texture.h"
#include "mesh.h"
#include "Component.h"

class Game;
class RenderObj
{
public:
	RenderObj(Graphics* graphics,
		const Mesh* pMesh,
		Game* game
	);
	virtual ~RenderObj();
	virtual void Draw();
	struct PerObjectConstants {
		Matrix4 m_ModelToWorld;
	} m_ModelToWorldObj;
	void AddComponent(Component* pComp);
	virtual void Update(float deltaTime);
	Game* GetGame();

protected:
	Graphics* m_Graphics;
	ID3D11Buffer* m_ObjectBuffer;
	const Mesh* m_Mesh;
	std::vector<Component*> m_Components;
	Game* m_Game;
};

