#include "stdafx.h"
#include "CollisionComponent.h"
#include "RenderObj.h"
#include "../Source/game.h"

CollisionComponent::CollisionComponent(RenderObj* obj):
	Component(obj)
{
	obj->GetGame()->GetPhysics()->AddObj(this);
}

CollisionComponent::~CollisionComponent()
{
	mObj->GetGame()->GetPhysics()->RemoveObj(this);
}

void CollisionComponent::LoadProperties(const rapidjson::Value& properties)
{
	// Read the AABB min and AABB max
	GetVectorFromJSON(properties, "min", m_AABB.m_MinimumCorner);
	GetVectorFromJSON(properties, "max", m_AABB.m_MaximumCorner);
}

Physics::AABB CollisionComponent::GetAABB() const
{
	// Extract scale and translation from the render obj.
	float scale = mObj->m_ModelToWorldObj.m_ModelToWorld.GetScale().x;
	Vector3 trans = mObj->m_ModelToWorldObj.m_ModelToWorld.GetTranslation();

	// Create the return AABB and set values.
	Physics::AABB returnAABB = m_AABB;
	returnAABB.m_MaximumCorner *= scale;
	returnAABB.m_MinimumCorner *= scale;
	returnAABB.m_MaximumCorner += trans;
	returnAABB.m_MinimumCorner += trans;

	// Return AABB.
	return returnAABB;
}
