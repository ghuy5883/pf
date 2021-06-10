#pragma once
#include "Component.h"
class SimpleRotate :
	public Component
{
	public:
		SimpleRotate(RenderObj* obj);
		~SimpleRotate();
		void LoadProperties(const rapidjson::Value& properties);
		void Update(float deltaTime);

	private:
		float m_Speed;
};

