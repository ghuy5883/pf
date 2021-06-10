#pragma once
#include "RenderObj.h"
#include "assetManager.h"

class RenderCube :
	public RenderObj
{
public:
	RenderCube(AssetManager* pAssetManager, const Shader* pShader, const
		Texture* pTex);
	~RenderCube();
	void Draw();

private:
	VertexBuffer* m_VertexBuffer;
	Mesh* m_Mesh;
};

