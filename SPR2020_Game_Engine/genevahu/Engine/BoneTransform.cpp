#include "stdafx.h"
#include "BoneTransform.h"

BoneTransform::BoneTransform()
{
}

BoneTransform::~BoneTransform()
{
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform interpolate;
	interpolate.mRotation = Slerp(a.mRotation, b.mRotation, f);
	interpolate.mTranslation = Lerp(a.mTranslation, b.mTranslation, f);

	return interpolate;
}

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 a = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 b = Matrix4::CreateTranslation(mTranslation);

	return a * b;
}
