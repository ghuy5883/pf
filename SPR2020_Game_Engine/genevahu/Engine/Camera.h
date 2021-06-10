#pragma once
#include "engineMath.h"
#include "Graphics.h"
#include <d3d11.h>

class Camera
{
public:
	Camera(Graphics* graphics);
	virtual ~Camera();
	void SetActive();
	struct PerCameraConstants {
		Matrix4 c_viewProj;
		Vector3 c_cameraPosition;
		float padding = 1;
	};
	void SetCameraPositionAndRotation(Vector3 pos, Quaternion rot);
	void SetViewMat(Matrix4 mat);

protected:
	Graphics* m_Graphics;
	ID3D11Buffer* m_CameraBuffer;
	Matrix4 m_WorldToCamera;
	Matrix4 m_ProjectionMatrix;
    PerCameraConstants m_CameraObj;

private:

};

