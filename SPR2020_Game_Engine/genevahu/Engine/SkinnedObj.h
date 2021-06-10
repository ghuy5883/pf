#pragma once
#include "RenderObj.h"
class SkinnedObj :
	public RenderObj
{
	public:
		#define MAX_SKELETON_BONES 80
		struct SkinConstants {
			Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
		} m_SkinConstants;

		SkinnedObj(Graphics* graphics,
			const Mesh* pMesh,
			Game* game);
		~SkinnedObj();
		void Draw();

	private:
		ID3D11Buffer* m_SkinningMatrixBuffer;
};

