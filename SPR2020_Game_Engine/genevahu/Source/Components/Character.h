#pragma once
#include "../Engine/Component.h"
#include "../game.h"

class Character :
	public Component
{
public:
	Character(SkinnedObj* obj);
	~Character();
	void LoadProperties(const rapidjson::Value& properties);
	void UpdateAnim(float deltaTime);
	void Update(float deltaTime);
	bool SetAnim(const std::string& animName);

protected:
	SkinnedObj* m_SkinnedObj;
	Skeleton* m_Skeleton;
	const Animation* m_CurrentAnimation;
	std::unordered_map<std::string, const Animation*> m_Animations;
	float m_CurrentAnimationTime;
};
