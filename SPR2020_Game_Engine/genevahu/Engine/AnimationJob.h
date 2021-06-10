#pragma once
#include "Job.h"
#include "Animation.h"
#include "SkinnedObj.h"
class AnimationJob :
	public Job
{
	public:
		AnimationJob(const Animation* anim, Skeleton* skel, SkinnedObj* obj, float animTime);
		~AnimationJob();
		void DoIt();

	private:
		const Animation* m_CurrentAnimation;
		Skeleton* m_Skeleton;
		SkinnedObj* m_SkinnedObj;
		float m_CurrentAnimationTime;
};

