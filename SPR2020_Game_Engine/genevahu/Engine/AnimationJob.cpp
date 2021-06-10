#include "stdafx.h"
#include "AnimationJob.h"

AnimationJob::AnimationJob(const Animation* anim, Skeleton* skel, SkinnedObj* obj, float animTime)
{
	m_SkinnedObj = obj;
	m_CurrentAnimation = anim;
	m_Skeleton = skel;
	m_CurrentAnimationTime = animTime;
}

AnimationJob::~AnimationJob()
{
}

void AnimationJob::DoIt()
{
	// Get pose matrices
	std::vector<Matrix4> animationMatrices;
	m_CurrentAnimation->GetGlobalPoseAtTime(animationMatrices, m_Skeleton, m_CurrentAnimationTime);

	// Combine resulting matrices with inverse bind pose matrices
	std::vector<Matrix4> invBindposeMatrices = m_Skeleton->GetGlobalInvBindPoses();
	for (int i = 0; i < animationMatrices.size(); i++) {
		m_SkinnedObj->m_SkinConstants.c_skinMatrix[i] = invBindposeMatrices[i] * animationMatrices[i];
	}
}
