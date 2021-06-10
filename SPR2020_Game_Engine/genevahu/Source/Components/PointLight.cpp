#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight(RenderObj* obj)
	: Component(obj)
{
	mObj = obj;
	m_pLightData = mObj->GetGame()->AllocateLight();
}

PointLight::~PointLight()
{
	mObj->GetGame()->FreeLight(m_pLightData);
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
	// Variables to instantiate from the loaded JSON.
	Vector3 diffuseColor;
	Vector3 specularColor;
	float specularPower;
	float innerRadius;
	float outerRadius;

	GetVectorFromJSON(properties, "diffuseColor", diffuseColor);
	GetVectorFromJSON(properties, "specularColor", specularColor);
	GetFloatFromJSON(properties, "specularPower", specularPower);
	GetFloatFromJSON(properties, "innerRadius", innerRadius);
	GetFloatFromJSON(properties, "outerRadius", outerRadius);

	// Set values in point light data.
	m_pLightData->diffuseColor = diffuseColor;
	m_pLightData->specularColor = specularColor;
	m_pLightData->specularPower = specularPower;
	m_pLightData->innerRadius = innerRadius;
	m_pLightData->outerRadius = outerRadius;
}

void PointLight::Update(float deltaTime)
{
	// Update position
	m_pLightData->position = mObj->m_ModelToWorldObj.m_ModelToWorld.GetTranslation();
}

