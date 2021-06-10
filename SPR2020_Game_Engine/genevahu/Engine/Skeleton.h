#pragma once
#include "BoneTransform.h"

class AssetManager;
class Skeleton
{
	public:	
		Skeleton(AssetManager* pAssetManager);
		~Skeleton();

		struct Bone {
			BoneTransform mBoneTransform;
			std::string mName;
			int mParentIndex;
		};
		
		size_t GetNumBones() const;
		const Bone& GetBone(size_t idx) const;
		const std::vector<Bone>& GetBones() const;
		const std::vector<Matrix4>& GetGlobalInvBindPoses() const;
		static Skeleton* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
		bool Load(const WCHAR* fileName);

	private:
		std::vector<Bone> mBones;
		std::vector<Matrix4> mGlobalInverseBindPoseMatrices;
		void ComputeGlobalInvBindPose();
		AssetManager* mAssetManager;
};

