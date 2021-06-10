#include "stdafx.h"
#include <codecvt>
#include <locale>
#include "Character.h"
#include "AnimationJob.h"

Character::Character(SkinnedObj* obj)
	: Component(obj)
{
	m_CurrentAnimation = nullptr;
	m_SkinnedObj = obj;
	m_Skeleton = nullptr;
	m_CurrentAnimationTime = 0.0f;
}

Character::~Character()
{
}

void Character::LoadProperties(const rapidjson::Value& properties)
{
	// Get skeleton filename
	const char* skeletonName = properties["skeleton"].GetString();
	std::wstring wskeletonName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(skeletonName);
	// Load skeleton
	m_Skeleton = mObj->GetGame()->GetAssetManager()->LoadSkeleton(wskeletonName);

	// Get the array of animations
	const rapidjson::Value& animations = properties["animations"];
	if (!animations.IsArray()) {
		return;
	}
	// Iterate through the array of animations to get the animation name and filename
	for (rapidjson::SizeType i = 0; i < animations.Size(); i++) {
		std::string animName = animations[i][0].GetString();
		std::string fileName = animations[i][1].GetString();
		std::wstring wfileName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(fileName);

		//Load the animation and enter it into the map
		Animation* newAnimation = mObj->GetGame()->GetAssetManager()->LoadAnimation(wfileName);
		m_Animations[animName] = newAnimation;
	}

}

void Character::UpdateAnim(float deltaTime)
{
	// Check if there is a current animation
	if (m_CurrentAnimation != nullptr) {
		float upcomingAnimTime = deltaTime + m_CurrentAnimationTime;
		// Wrap timer if it exceeds the animation's length
		if (upcomingAnimTime > m_CurrentAnimation->GetLength()) {
			// Float modulus to get amount of time that exceeded the animation's length
			upcomingAnimTime = fmod(upcomingAnimTime, m_CurrentAnimation->GetLength());
		}
		m_CurrentAnimationTime = upcomingAnimTime;

		/*
		// Get pose matrices
		std::vector<Matrix4> animationMatrices;
		m_CurrentAnimation->GetGlobalPoseAtTime(animationMatrices, m_Skeleton,m_CurrentAnimationTime);
	
		// Combine resulting matrices with inverse bind pose matrices
		std::vector<Matrix4> invBindposeMatrices = m_Skeleton->GetGlobalInvBindPoses();
		for (int i = 0; i < animationMatrices.size(); i++) {
			m_SkinnedObj->m_SkinConstants.c_skinMatrix[i] = invBindposeMatrices[i] * animationMatrices[i];
		}
		*/

		// Create an animation job 
		AnimationJob* newAnimJob = new AnimationJob(m_CurrentAnimation, m_Skeleton, m_SkinnedObj, m_CurrentAnimationTime);
		mObj->GetGame()->GetJobManager()->AddJob(newAnimJob);
	}
	
}

void Character::Update(float deltaTime)
{
	if (m_CurrentAnimation == nullptr) {
		SetAnim("idle");
	}
	UpdateAnim(deltaTime);
}

bool Character::SetAnim(const std::string& animName)
{
	// Use find function to locate the animation
	std::unordered_map<std::string, const Animation*>::const_iterator it = m_Animations.find(animName);
	if (it != m_Animations.end()) {
		// Set this animation as current
		m_CurrentAnimation = it->second;
		// Set animation timer to 0.0
		m_CurrentAnimationTime = 0.0f;

		return true;
	}
	return false;
}
