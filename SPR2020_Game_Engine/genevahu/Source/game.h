#pragma once
#include "Graphics.h"
#include "VertexBuffer.h"
#include "SkinnedObj.h"
#include "Camera.h"
#include "texture.h"
#include "assetManager.h"
#include "JobManager.h"
#include "Physics.h"

class Shader;

class Game
{
public:
    Game();
    ~Game();

	// Game loop-related stuff
    void Init(HWND hWnd, float width, float height);
    void Shutdown();
	void Update(float deltaTime);

	// Graphics-related stuff
    void RenderFrame();

	// Input-related stuff
	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

	// Lighting-related stuff
	#define MAX_POINT_LIGHTS 8
	struct PointLightData {
		// 16-byte aligned, block 1 (0-15)
		Vector3 diffuseColor = Vector3(0.0f, 0.0f, 0.0f);
		float padding0 = 42.0f;

		// Block 2 (16-31)
		Vector3 specularColor = Vector3(0.0f, 0.0f, 0.0f);
		float padding1 = 42.0f;

		// Block 3 (32 - 47)
		Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
		float specularPower = 42.0f;

		// Block 4
		float innerRadius = 0.0f;
		float outerRadius = 0.0f;
		bool isEnabled = false;
		float padding2 = 42.0f;
	};

	struct LightingConstants {
		// 16-byte aligned, block 1
		Vector3 c_ambient;
		float padding = 0.0f;

		// An array of up to 8 point lights, block 2
		PointLightData c_pointLight[MAX_POINT_LIGHTS];
	};

	PointLightData* AllocateLight();
	void FreeLight(PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight() const;
	AssetManager* GetAssetManager() const;
	JobManager* GetJobManager() const;
	Camera* GetCamera() const;
	Physics* GetPhysics() const;

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	// Graphics 
	Graphics m_Graphics;
	// Vertex buffer
	VertexBuffer* m_VertexBuffer;
	// Camera
	Camera* m_Camera;
	// Lighting
	LightingConstants m_Lighting;
	ID3D11Buffer* m_LightingBuffer;
	// Asset manager
	AssetManager* m_AssetManager;
	// Job manager
	JobManager* m_JobManager;
	// Physics instance
	Physics* m_Physics;
	// List of render objects
	std::vector<RenderObj*> m_RenderObjs;
	// Render target texture
	Texture* m_RenderTex;
	Texture* m_RenderTexHalf;
	Texture* m_RenderTexQuarter;
	Texture* m_RenderTexQuarter2;
	bool LoadLevel(const WCHAR* fileName);
};