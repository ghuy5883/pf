#include "stdafx.h"
#include "SimpleRotate.h"
#include "RenderObj.h"

SimpleRotate::SimpleRotate(RenderObj* obj)
	: Component(obj)
{
}

SimpleRotate::~SimpleRotate()
{
}

void SimpleRotate::LoadProperties(const rapidjson::Value& properties)
{
	m_Speed = properties["speed"].GetDouble();
}

void SimpleRotate::Update(float deltaTime)
{
	mObj->m_ModelToWorldObj.m_ModelToWorld *= mObj->m_ModelToWorldObj.m_ModelToWorld.CreateRotationZ(deltaTime * m_Speed);
}
