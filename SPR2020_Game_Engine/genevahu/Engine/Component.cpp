#include "stdafx.h"
#include "Component.h"
#include "RenderObj.h"

Component::Component(RenderObj* rObj)
{
	mObj = rObj;
}

Component::~Component()
{
}

void Component::LoadProperties(const rapidjson::Value& properties)
{
}

void Component::Update(float deltaTime)
{
}
