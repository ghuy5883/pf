#pragma once
#include "Component.h"
#include "Physics.h"

class CollisionComponent : 
	public Component
{
	public:
		CollisionComponent(RenderObj* obj);
		~CollisionComponent();
		void LoadProperties(const rapidjson::Value& properties);
		Physics::AABB GetAABB() const;

	protected:
		Physics::AABB m_AABB;
};

