#pragma once
#include "../Engine/Component.h"
#include "../game.h"

class PointLight :
	public Component
{
public:
	PointLight(RenderObj* obj);
	~PointLight();
	void LoadProperties(const rapidjson::Value& properties);
	void Update(float deltaTime);

private:
	Game::PointLightData* m_pLightData;
};

