#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Graphics* graphics)
{
	//Initialize variables with arguments
	m_Graphics = graphics;

	m_WorldToCamera = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, -100.0f));
	m_ProjectionMatrix = Matrix4::CreateRotationY(-Math::PiOver2) *
		Matrix4::CreateRotationZ(-Math::PiOver2) *
		Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
			m_Graphics->GetScreenWidth(),
			m_Graphics->GetScreenHeight(),
			25.0f,
			10000.0f);
	m_CameraBuffer = m_Graphics->CreateGraphicsBuffer(&m_CameraObj, sizeof(m_CameraObj), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Camera::~Camera()
{
	m_CameraBuffer->Release();
}

void Camera::SetActive()
{
	// Fill c_viewProj with WorldToCamera times the projection.
	m_CameraObj.c_viewProj = m_WorldToCamera * m_ProjectionMatrix;
	// Get the camera's position.
	// Don't actually use the world camera matrix, so use a dummy matrix. The inverse of the world to camera --> camera in the world
	Matrix4 saveCamera = m_WorldToCamera;
	saveCamera.Invert();
	m_CameraObj.c_cameraPosition = saveCamera.GetTranslation();
	// Fill the constant buffer.
	m_Graphics->UploadBuffer(m_CameraBuffer, &m_CameraObj, sizeof(m_CameraObj));
	// Bind the constant buffer.
	m_Graphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &m_CameraBuffer);
	// Save for pixels buffer later.
	m_Graphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_CAMERA, 1, &m_CameraBuffer);
}

void Camera::SetCameraPositionAndRotation(Vector3 pos, Quaternion rot)
{
	// Inverse the matrix created using the camera's world position.
	m_WorldToCamera = Matrix4::CreateRotationX(rot.x) * 
		Matrix4::CreateRotationY(rot.y) *
		Matrix4::CreateRotationZ(rot.z) *
		Matrix4::CreateTranslation(pos);
	m_WorldToCamera.Invert();
}

void Camera::SetViewMat(Matrix4 mat)
{
	m_WorldToCamera = mat;
}
