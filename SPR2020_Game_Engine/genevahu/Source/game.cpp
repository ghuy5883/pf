#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "stringUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "jsonUtil.h"
#include "Profiler.h"
#include "Components/PointLight.h"
#include "Components/Character.h"
#include "Components/player.h"
#include "Components/followCam.h"
#include "CollisionComponent.h"
#include "SimpleRotate.h"

float rotationAngle = 1.0f;
struct VertexPosColor
{
	Vector3 pos;
	Graphics::Color4 color;
};
struct VertexPosColorUVNormal
{
	Vector3 pos;
	Vector3 normal;
	Graphics::Color4 color;
	Vector2 uv;
};
struct VertexPosUVNormal
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};
struct VertexSkinned
{
	Vector3 pos;
	Vector3 normal;
	uint8_t bones[4];
	unsigned char weights[4];
	Vector2 uv;
};
struct VertexNormal
{
	Vector3 pos;
	Vector3 normal;
	Vector3 tangent;
	Vector2 uv;
};
struct TextureCopy
{
	Vector3 pos;
	Vector2 uv;
};


Game::Game()
{
	m_Camera = nullptr;
	m_VertexBuffer = nullptr;
	m_LightingBuffer = nullptr;
	m_AssetManager = nullptr;
	m_Physics = nullptr;
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	/* -------------------------------------------------------------------------------- */
		// Setup for the Graphics object.
	/* -------------------------------------------------------------------------------- */
	//Initialize graphics.
	m_Graphics.InitD3D(hWnd, width, height);

	/* -------------------------------------------------------------------------------- */
		// Setup for asset manager.
	/* -------------------------------------------------------------------------------- */
	m_AssetManager = new AssetManager(&m_Graphics);

	/* -------------------------------------------------------------------------------- */
		// Setup for job manager.
	/* -------------------------------------------------------------------------------- */
	m_JobManager = new JobManager();

	/* -------------------------------------------------------------------------------- */
		// Setup for physics.
	/* -------------------------------------------------------------------------------- */
	m_Physics = new Physics();

	/* -------------------------------------------------------------------------------- */
		// Setup for the shader.
	/* -------------------------------------------------------------------------------- */
	// Create the shader.
	Shader* m_Shader = new Shader(&m_Graphics);
	Shader* m_Shader_UVNormal = new Shader(&m_Graphics);
	Shader* m_Shader_UVNormalUnlit = new Shader(&m_Graphics);
	Shader* m_Shader_UVColorNormal = new Shader(&m_Graphics);
	Shader* m_Shader_Skinned = new Shader(&m_Graphics);
	Shader* m_Shader_Normal = new Shader(&m_Graphics);
	Shader* m_Shader_TextureCopy = new Shader(&m_Graphics);
	Shader* m_Shader_BloomMask = new Shader(&m_Graphics);
	Shader* m_Shader_GaussianHor = new Shader(&m_Graphics);
	Shader* m_Shader_GaussianVert = new Shader(&m_Graphics);

	// Creating description to use when loading the shader.
	D3D11_INPUT_ELEMENT_DESC layout_color[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosColor, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC layout_uv_normal[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosUVNormal, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUVNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosUVNormal, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC layout_uv_color_normal[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, offsetof(VertexPosColorUVNormal, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUVNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColorUVNormal, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosColorUVNormal, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC layout_skinned[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexSkinned, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexSkinned, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(VertexSkinned, bones), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(VertexSkinned, weights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexSkinned, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC layout_normal[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexNormal, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexNormal, tangent), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexNormal, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3D11_INPUT_ELEMENT_DESC layout_texturecopy[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(TextureCopy, pos),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(TextureCopy, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load the shader
	m_Shader->Load(L"Shaders/Mesh.hlsl", layout_color, (sizeof(layout_color) / sizeof(layout_color[0])));
	m_Shader_UVNormal->Load(L"Shaders/Phong.hlsl", layout_uv_normal, (sizeof(layout_uv_normal) / sizeof(layout_uv_normal[0])));
	m_Shader_UVNormalUnlit->Load(L"Shaders/Unlit.hlsl", layout_uv_normal, (sizeof(layout_uv_normal) / sizeof(layout_uv_normal[0])));
	m_Shader_UVColorNormal->Load(L"Shaders/BasicMesh.hlsl", layout_uv_color_normal, (sizeof(layout_uv_color_normal) / sizeof(layout_uv_color_normal[0])));
	m_Shader_Skinned->Load(L"Shaders/Skinned.hlsl", layout_skinned, (sizeof(layout_skinned) / sizeof(layout_skinned[0])));
	m_Shader_Normal->Load(L"Shaders/Normal.hlsl", layout_normal, (sizeof(layout_normal) / sizeof(layout_normal[0])));
	m_Shader_TextureCopy->Load(L"Shaders/Copy.hlsl", layout_texturecopy, (sizeof(layout_texturecopy) / sizeof(layout_texturecopy[0])));
	m_Shader_BloomMask->Load(L"Shaders/BloomMask.hlsl", layout_texturecopy, (sizeof(layout_texturecopy) / sizeof(layout_texturecopy[0])));
	m_Shader_GaussianHor->Load(L"Shaders/GaussianHorizontal.hlsl", layout_texturecopy, (sizeof(layout_texturecopy) / sizeof(layout_texturecopy[0])));
	m_Shader_GaussianVert->Load(L"Shaders/GaussianVertical.hlsl", layout_texturecopy, (sizeof(layout_texturecopy) / sizeof(layout_texturecopy[0])));

	m_AssetManager->SetShader(L"Mesh", m_Shader);
	m_AssetManager->SetShader(L"Phong", m_Shader_UVNormal);
	m_AssetManager->SetShader(L"Unlit", m_Shader_UVNormalUnlit);
	m_AssetManager->SetShader(L"BasicMesh", m_Shader_UVColorNormal);
	m_AssetManager->SetShader(L"Skinned", m_Shader_Skinned);
	m_AssetManager->SetShader(L"Normal", m_Shader_Normal);
	m_AssetManager->SetShader(L"Copy", m_Shader_TextureCopy);
	m_AssetManager->SetShader(L"BloomMask", m_Shader_BloomMask);
	m_AssetManager->SetShader(L"GaussianHor", m_Shader_GaussianHor);
	m_AssetManager->SetShader(L"GaussianVert", m_Shader_GaussianVert);

	/* -------------------------------------------------------------------------------- */
		// Setup for the lighting.
	/* -------------------------------------------------------------------------------- */
	// Create the lighting buffer.
	m_LightingBuffer = m_Graphics.CreateGraphicsBuffer(&m_Lighting, sizeof(m_Lighting), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	/* -------------------------------------------------------------------------------- */
		// Setup for the level.
	/* -------------------------------------------------------------------------------- */
	// Load the level.
	LoadLevel(L"Assets/Levels/Level10.itplevel");

	/* -------------------------------------------------------------------------------- */
		// Setup for full-screen pass.
	/* -------------------------------------------------------------------------------- */
	// Create texture and matching render target
	m_RenderTex = new Texture(&m_Graphics);
	m_Graphics.SetRenderTextureView(m_RenderTex->CreateRenderTarget(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT));

	m_RenderTexHalf = new Texture(&m_Graphics);
	m_Graphics.SetRenderTextureViewHalf(m_RenderTexHalf->CreateRenderTarget(width/2.0f, height/2.0f, DXGI_FORMAT_R32G32B32A32_FLOAT));

	m_RenderTexQuarter = new Texture(&m_Graphics);
	m_Graphics.SetRenderTextureViewQuarter(m_RenderTexQuarter->CreateRenderTarget(width/4.0f, height/4.0f, DXGI_FORMAT_R32G32B32A32_FLOAT));

	m_RenderTexQuarter2 = new Texture(&m_Graphics);
	m_Graphics.SetRenderTextureViewQuarter2(m_RenderTexQuarter2->CreateRenderTarget(width / 4.0f, height / 4.0f, DXGI_FORMAT_R32G32B32A32_FLOAT));

	// Create full-screen quad: 4 vertices, 2 triangles.
	static TextureCopy cubeVertex[] =
	{
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) }
	};

	static uint16_t cubeIndex[] = {
		0, 1, 3,
		1, 2, 3
	};

	m_VertexBuffer = new VertexBuffer(&m_Graphics, cubeVertex, sizeof(cubeVertex), sizeof(cubeVertex[0]), cubeIndex, sizeof(cubeIndex), sizeof(uint16_t));

	// Set up blend states.
	m_Graphics.SetBlendOpaque(m_Graphics.CreateBlendState(false, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO));
	m_Graphics.SetBlendAdditive(m_Graphics.CreateBlendState(true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE));

}

void Game::Shutdown()
{
	// Release the rendered objects.
	for (int i = 0; i < m_RenderObjs.size(); i++) {
		delete m_RenderObjs[i];
	}

	// Clear the asset manager
	m_AssetManager->Clear();

	// Delete the job manager.
	delete m_JobManager;

	// Delete the physics.
	delete m_Physics;

	// Delete the camera.
	delete m_Camera;

	// Release the lighting.
	m_LightingBuffer->Release();

	// Delete the render texture.
	delete m_RenderTex;
	delete m_RenderTexHalf;
	delete m_RenderTexQuarter;
	delete m_RenderTexQuarter2;

	// Delete off-screen vertex buffer.
	delete m_VertexBuffer;

	// Clean up the graphics class.
	m_Graphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	// Timer for the update process.
	PROFILE_SCOPE(Update);

	// Update all render objects.
	for (int i = 0; i < m_RenderObjs.size(); i++) {
		m_RenderObjs[i]->Update(deltaTime);
	}

	// Wait for jobs.
	m_JobManager->WaitForJobs();
}

void Game::RenderFrame()
{
	/* -------------------------------------------------------------------------------- */
		//Render objects in the scene.
	/* -------------------------------------------------------------------------------- */
	// Timer for the frame render.
	PROFILE_SCOPE(FrameRender);

	// Begin the frame with the preset color.
	{
		PROFILE_SCOPE(BeginFrameRender);
		m_Graphics.SetRenderTarget(m_Graphics.GetRenderTexture(), m_Graphics.GetDepthStencilView());
		m_Graphics.BeginFrame(Graphics::Color4(0.0f, 0.2f, 0.4f, 1.0f));
		m_Graphics.SetBlendState(m_Graphics.GetBlendOpaque());
	}

	// Activate the camera.
	{
		PROFILE_SCOPE(ActivateCamera);
		m_Camera->SetActive();
	}

	// Activate the lighting.
	{
		PROFILE_SCOPE(RenderLighting);
		m_Graphics.UploadBuffer(m_LightingBuffer, &m_Lighting, sizeof(m_Lighting));

		// Set the lighting buffer.
		m_Graphics.GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHTING, 1, &m_LightingBuffer);

	}

	// Draw render objects.
	for (int i = 0; i < m_RenderObjs.size(); i++) {
		PROFILE_SCOPE(DrawObjects);
		m_RenderObjs[i]->Draw();
	}

	/* -------------------------------------------------------------------------------- */
		//Down-sample main render into smaller buffers.
	/* -------------------------------------------------------------------------------- */
	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Set the half-by-half buffer as the render target.
	m_Graphics.SetViewport(0.0f, 0.0f, m_Graphics.GetScreenWidth() / 2.0f, m_Graphics.GetScreenHeight() / 2.0f);
	m_Graphics.SetRenderTarget(m_Graphics.GetRenderTextureHalf(), nullptr);

	// Copy off-screen buffer to the half-by-half buffer.
	m_RenderTex->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"BloomMask")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Set the quarter-by-quarter buffer as the render target.
	m_Graphics.SetViewport(0.0f, 0.0f, m_Graphics.GetScreenWidth() / 4.0f, m_Graphics.GetScreenHeight() / 4.0f);
	m_Graphics.SetRenderTarget(m_Graphics.GetRenderTextureQuarter(), nullptr);

	// Copy half-by-half buffer to the quarter-by-quarter buffer.
	m_RenderTexHalf->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"Copy")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Set the second quarter-by-quarter buffer as the render target.
	m_Graphics.SetRenderTarget(m_Graphics.GetRenderTextureQuarter2(), nullptr);

	// Horizontal pass from first quarter-by-quarter buffer to the second quarter-by-quarter buffer.
	m_RenderTexQuarter->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"GaussianHor")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Set the first quarter-by-quarter buffer as the render target.
	m_Graphics.SetRenderTarget(m_Graphics.GetRenderTextureQuarter(), nullptr);

	// Vertical pass from second quarter-by-quarter buffer to the first quarter-by-quarter buffer.
	m_RenderTexQuarter2->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"GaussianVert")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Set the back buffer as the render target.
	m_Graphics.SetViewport(0.0f, 0.0f, m_Graphics.GetScreenWidth(), m_Graphics.GetScreenHeight());
	m_Graphics.SetRenderTarget(m_Graphics.GetBackBuffer(), nullptr);

	// Copy quarter-by-quarter buffer to the back buffer.
	m_RenderTexQuarter->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"Copy")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
		// Set blend state to additive.
	m_Graphics.SetBlendState(m_Graphics.GetBlendAdditive());

	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	// Copy off-screen buffer to the back buffer.
	m_RenderTex->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);
	m_AssetManager->GetShader(L"Copy")->SetActive();
	m_VertexBuffer->Draw();

	// Unbind resources. -------------------------------------------------------------------
	for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; i++) {
		m_Graphics.SetActiveTexture(i, nullptr);
	}

	//End the frame. -----------------------------------------------------------------------
	{
		PROFILE_SCOPE(PresentRenderedFrame);
		m_Graphics.EndFrame();
	}
	
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

Game::PointLightData* Game::AllocateLight()
{
	bool isEnabled = true;

	// Find the point light in the array that has isEnabled set to false.
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		isEnabled = m_Lighting.c_pointLight[i].isEnabled;

		if (!isEnabled) {
			m_Lighting.c_pointLight[i].isEnabled = true;
			return &m_Lighting.c_pointLight[i];
		}
	}

	return nullptr;
}

void Game::FreeLight(PointLightData* pLight)
{
	// Set isEnabled to false.
	pLight->isEnabled = false;
}

void Game::SetAmbientLight(const Vector3& color)
{
	m_Lighting.c_ambient = color;
}

const Vector3& Game::GetAmbientLight() const
{
	return m_Lighting.c_ambient;
}

AssetManager* Game::GetAssetManager() const
{
	return m_AssetManager;
}

JobManager* Game::GetJobManager() const
{
	return m_JobManager;
}

Camera* Game::GetCamera() const
{
	return m_Camera;
}

Physics* Game::GetPhysics() const
{
	return m_Physics;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	// Load camera data
	const rapidjson::Value& cameraFormat = doc["camera"];
	
	Vector3 cameraPos;
	Quaternion cameraRot;
	GetVectorFromJSON(cameraFormat, "position", cameraPos);
	GetQuaternionFromJSON(cameraFormat, "rotation", cameraRot);
	m_Camera = new Camera(&m_Graphics);
	m_Camera->SetCameraPositionAndRotation(cameraPos, cameraRot);

	// Load lighting
	const rapidjson::Value& lightingFormat = doc["lightingData"];

	Vector3 lightingAmbient;
	GetVectorFromJSON(lightingFormat, "ambient", lightingAmbient);
	SetAmbientLight(lightingAmbient);


	// Load objects
	const rapidjson::Value& renderObjectFormat = doc["renderObjects"];

	for (rapidjson::SizeType i = 0; i < renderObjectFormat.Size(); i++) {
		const rapidjson::Value& renderObject = renderObjectFormat[i];

		// Variables to instantiate
		Vector3 renderObjPos;
		Quaternion renderObjRot;
		float renderObjScale;
		std::wstring renderObjMeshName;
		
		// Populate the variables
		GetVectorFromJSON(renderObject, "position", renderObjPos);
		GetQuaternionFromJSON(renderObject, "rotation", renderObjRot);
		GetFloatFromJSON(renderObject, "scale", renderObjScale);
		GetWStringFromJSON(renderObject, "mesh", renderObjMeshName);

		// Use previously instantiated variables to create model to world matrix
		Matrix4 objToWorld = Matrix4::CreateScale(renderObjScale)
			* Matrix4::CreateRotationZ(renderObjRot.z)
			* Matrix4::CreateRotationY(renderObjRot.y)
			* Matrix4::CreateRotationX(renderObjRot.x)
			* Matrix4::CreateTranslation(renderObjPos);

		// Load mesh and check if it's skinned. 
		if (m_AssetManager->LoadMesh(renderObjMeshName)->IsSkinned() == true) {
			// Make a render object using the variables
			SkinnedObj* newRenderObj = new SkinnedObj(&m_Graphics, m_AssetManager->LoadMesh(renderObjMeshName), this);
			newRenderObj->m_ModelToWorldObj.m_ModelToWorld = objToWorld;

			// Handle any components
			const rapidjson::Value& renderObjComponents = renderObject["components"];
			if (renderObjComponents.Size() > 0) {
				for (rapidjson::SizeType i = 0; i < renderObjComponents.Size(); i++) {
					const rapidjson::Value& renderObjComponentProperties = renderObjComponents[i];
					std::string checkType;
					GetStringFromJSON(renderObjComponentProperties, "type", checkType);
					if (checkType == "Character") {
						// Create character.
						Character* pChar = new Character(newRenderObj);
						pChar->LoadProperties(renderObjComponentProperties);
						// Add component to the render object.
						newRenderObj->AddComponent(pChar);
					}
					else if (checkType == "Player") {
						// Create player.
						Player* pPlay = new Player(newRenderObj);
						pPlay->LoadProperties(renderObjComponentProperties);
						// Add component to the render object.
						newRenderObj->AddComponent(pPlay);
					}
					else if (checkType == "FollowCam") {
						// Create follow camera
						FollowCam* pFCam = new FollowCam(newRenderObj);
						pFCam->LoadProperties(renderObjComponentProperties);
						// Add component to the render object.
						newRenderObj->AddComponent(pFCam);
					}
				}	
			}

			m_RenderObjs.push_back(newRenderObj);
		}
		else {
			// Make a render object using the variables
			RenderObj* newRenderObj = new RenderObj(&m_Graphics, m_AssetManager->LoadMesh(renderObjMeshName), this);
			newRenderObj->m_ModelToWorldObj.m_ModelToWorld = objToWorld;

			// Handle any components
			const rapidjson::Value& renderObjComponents = renderObject["components"];
			if (renderObjComponents.Size() > 0) {
				for (rapidjson::SizeType i = 0; i < renderObjComponents.Size(); i++) {
					const rapidjson::Value& renderObjComponentProperties = renderObjComponents[i];
					std::string checkType;
					GetStringFromJSON(renderObjComponentProperties, "type", checkType);
					if (checkType == "PointLight") {
						// Create point light.
						PointLight* pLight = new PointLight(newRenderObj);
						pLight->LoadProperties(renderObjComponentProperties);
						// Add component to the render object.
						newRenderObj->AddComponent(pLight);
					}
					else if (checkType == "Collision") {
						// Create collision component.
						CollisionComponent* pColl = new CollisionComponent(newRenderObj);
						pColl->LoadProperties(renderObjComponentProperties);
						// Add component to the render object.
						newRenderObj->AddComponent(pColl);
					}
					else if (checkType == "SimpleRotate") {
						// Create rotate component.
						SimpleRotate* pRot = new SimpleRotate(newRenderObj);
						pRot->LoadProperties(renderObjComponentProperties);
						// Add component to render object.
						newRenderObj->AddComponent(pRot);
					}
				}
			}

			m_RenderObjs.push_back(newRenderObj);
		}
	}

	return true;
}