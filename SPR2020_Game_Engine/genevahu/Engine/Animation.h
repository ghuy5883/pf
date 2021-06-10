#pragma once
#include "Skeleton.h"

class AssetManager;
class Animation
{
	public:
		Animation(AssetManager* pAssetManager);
		~Animation();
		uint32_t GetNumBones() const;
		uint32_t GetNumFrames() const;
		float GetLength() const;
		static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
		bool Load(const WCHAR* fileName);
		void GetGlobalPoseAtTime(
			std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime
		) const;

	private:
		uint32_t m_NumBones;
		uint32_t m_NumFrames;
		float m_AnimLength;
		std::vector<std::vector<BoneTransform> > m_AnimData;
		AssetManager* m_AssetManager;

};

