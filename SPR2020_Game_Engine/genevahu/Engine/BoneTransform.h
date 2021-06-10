#pragma once
#include "engineMath.h"

class BoneTransform
{
	public:
		BoneTransform();
		~BoneTransform();
		Quaternion mRotation;
		Vector3 mTranslation;
		static BoneTransform Interpolate(
			const BoneTransform& a, const BoneTransform& b, float f);


		// Functions
		Matrix4 ToMatrix() const;
};

